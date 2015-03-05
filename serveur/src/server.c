/* 
 * DESCRIPTION DU FICHIER 
 * Name : server.c
 */

// --- INCLUDE ------------------------------------------------------------------ //
#include "server.h"
// --- INCLUDE ------------------------------------------------------------------ //

// --- ZONE-TEST----------------------------------------------------------------- //
int verifier_dossier_lancement(char *dossier_programme)
{
	char dossier_lancement[100];
	getcwd(dossier_lancement, 100);
	if (strcmp(dossier_lancement, dossier_programme) != 0)
	{ /* Si le dossier de lancement est différent du dossier d'installation du programme... */
		if (chdir(dossier_programme) == 0)
		{ /* On change le répertoire de lancement et on vérifie que tout se passe bien */
			return EXIT_SUCCESS;
		}
		else
		{
			return EXIT_FAILURE;
		}
	}
	else
	{
		return EXIT_SUCCESS;
	}
}
void dossier_lancement(char *chemin_programme, char *dossier)
{
	int i;
	for (i = strlen(chemin_programme); i>0; i--)
	{
		if (chemin_programme[i] == '/')
			/* Lorsqu'on tombe sur le premier "/", on commence à recopier chemin_programme dans dossier */
		{
			dossier[i] = '\0';
			i--; /* On passe au caractère précédent pour commencer à recopier le dossier de lancement */
			while (i >= 0)
			{
				dossier[i] = chemin_programme[i];
				i--;
			}
		}
	}
}
int traitement_register(char *cli_id)
{
    con=connection_bdd();
    // TODO ERR
    char request[1024];
    int pid;
    int retour=0;
    MYSQL_ROW row;
    MYSQL_RES *result=NULL;
    sprintf(request,
            "SELECT `cli_ip` FROM CLIENTS WHERE `cli_id`=\'%s\' ;",cli_id);
    // TODO : securiser injection SQL
    if(mysql_query(con,request))
    {
        syslog(LOG_ERR,"ERREUR SQL: %s\n",mysql_error(con));
        mysql_close(con);
        return 1001;// TODO -1
    }
    result = mysql_store_result(con);
    if(result==NULL)
    {
        syslog(LOG_ERR,"ERREUR SQL : %s\n",mysql_error(con));
        mysql_close(con);
        return 1002;
    }
    row=mysql_fetch_row(result);
    retour=init_connect(&sock_out,row[0],PORT_CLIENT_1);
    if(!retour)
    {
	pid=fork();
	if(pid==-1)
	{
	    syslog(LOG_ERR,"Problème lancement fork fct traitement_register : %s\n",strerror(errno));
	    close(sock_out.sock);
	    exit(-1);
	}
	if(pid==0)
	{
	    fermeture_socket_ecoute();
	    enregistrement_client(cli_id);
	    mysql_free_result(result);
	    mysql_close(con);
	    close(sock_out.sock);
	    exit(0);
	}
	lancement_thread_kill(pid);
	close(sock_out.sock);
    }
    mysql_free_result(result);
    mysql_close(con);
    return 0;

}
int recuperation_id_bdd()
{
	int i;
	int nbParam = 0;
	char buffer[1024];
	char splitParam[20][100];
	FILE* file = NULL;
	for (i = 0; i < 20; i++)
	{
	    memset(splitParam[i], 0, 100);
	}
	memset(user_bdd, 0, 100);
	memset(mdp_bdd, 0, 100);
	file = fopen("server.conf", "r");
	if (file == NULL)
	{
		syslog(LOG_ERR, "Ouverture du fichier de configuration impossible : %s", strerror(errno));
		return -1;
	}
	memset(buffer, 0, 1024);
	if (fgets(buffer, 1024, file) == NULL)
	{
		syslog(LOG_ERR, "Echec lors de la lecture du fichier de configuration");
		fclose(file);
		return -1;
	}
	nbParam = decoupageCommande(buffer, "=", splitParam);
	if (nbParam != 2)
	{
		syslog(LOG_ERR, "Echec lors de la lecture du fichier de configuration");
		fclose(file);
		return -1;
	}
	sprintf(user_bdd, "%s", splitParam[1]);
	for (i = 0; i < (int)strlen(user_bdd); i++)
	{
		if (user_bdd[i] == ';')
		{
			user_bdd[i] = 0;
		}
	}
	memset(buffer, 0, 1024);
	if (fgets(buffer, 1024, file) == NULL)
	{
		syslog(LOG_ERR, "Echec lors de la lecture du fichier de configuration");
		fclose(file);
		return -1;
	}
	for (i = 0; i < 20; i++)
	{
		memset(splitParam[i], 0, 100);
	}
	nbParam = decoupageCommande(buffer, "=", splitParam);
	if (nbParam != 2)
	{
		syslog(LOG_ERR, "Echec lors de la lecture du fichier de configuration");
		fclose(file);
		return -1;
	}
	sprintf(mdp_bdd, "%s", splitParam[1]);
	for (i = 0; i < (int)strlen(mdp_bdd); i++)
	{
		if (mdp_bdd[i] == ';')
		{
			mdp_bdd[i] = 0;
		}
	}
	fclose(file);
	return 0;
}
// --- ZONE-TEST----------------------------------------------------------------- //

// --- FONCTION MAIN ------------------------------------------------------------ //
int main(int argc, char* argv[])
{
	if (argc){}
	char dossier[100];
	dossier_lancement(argv[0], dossier);
	verifier_dossier_lancement(dossier);
    if(initialisation_serveur() == -1)
    {
		arret_serveur(-1);
    }
    lancement_thread_timer();
    fd_set rdfs;
    sigset_t empty_mask=set_masque_SIGINT();
    while(continu)
    {
	FD_ZERO(&rdfs);
	FD_SET(STDIN_FILENO,&rdfs);
	FD_SET(sock_in_1.sock,&rdfs);
	FD_SET(sock_in_2.sock,&rdfs);
	FD_SET(sock_web.sock,&rdfs);
	FD_SET(sock_in_unix.sock,&rdfs);
	FD_SET(internal.sock,&rdfs);
	if(pselect(internal.sock+1,&rdfs,NULL,NULL,NULL,&empty_mask)==-1)
	{
	    if(errno != EINTR)
	    {
		syslog(LOG_ERR,"%s\n",strerror(errno));
		arret_serveur(-1);
	    }
	}
	if(continu)
	{
	    if(FD_ISSET(STDIN_FILENO,&rdfs))
	    {
		traitement_entree_clavier();
	    }
	    if(FD_ISSET(sock_in_1.sock,&rdfs))
	    {
		traitement_entree_1();
	    }
	    if(FD_ISSET(sock_in_2.sock,&rdfs))
	    {
		traitement_entree_2();
	    }
	    if(FD_ISSET(sock_in_unix.sock,&rdfs))
	    {
		traitement_entree_unix();
	    }
	    if(FD_ISSET(sock_web.sock,&rdfs))
	    {
		traitement_entree_web();
	    }
	    if(FD_ISSET(internal.sock,&rdfs))
	    {
		traitement_entree_interne();
	    }
	}
    }
    return 0;
}
// --- FONCTION MAIN ------------------------------------------------------------ //

/**************************************************************************************
 *Gere les commandes passe par le cron de la meme maniere que                          *
 *traitement_entree_clavier                                                            *
 *                                                                                     *
 **************************************************************************************/

int traitement_entree_interne()
{
    char buffer[1024];
    ssize_t nb_bytes;
    int nbParam=0;
    char splitParam[20][100];
    nb_bytes=read(internal.sock,buffer,sizeof(buffer));
    if(nb_bytes == -1)
    {
	syslog(LOG_ERR,"%s",strerror(errno));
	return -1;
    }
    buffer[nb_bytes]=0;
    nbParam=decoupageCommande(buffer," ",splitParam);
    if(nbParam>0)
    {
	if(!strcmp(splitParam[0],"update"))
	{
	    traitement_update();
	}
	else if(!strcmp(splitParam[0],"add"))
	{
	    traitement_add(splitParam,nbParam);
	    traitement_show();
	}
	else if(!strcmp(splitParam[0],"del")
		|| !strcmp(splitParam[0],"rm"))
	{
	    traitement_del(splitParam,nbParam);
	    traitement_show();
	}
	else if(!strcmp(splitParam[0],"show")
		|| !strcmp(splitParam[0],"ls"))
	{
	    if(nbParam>1 && (!strcmp(splitParam[1],"script")
			|| !strcmp(splitParam[1],"scripts")
			|| !strcmp(splitParam[1],"sh")))
	    {
		traitement_show_script();
	    }
	    else
	    {
		traitement_show();
	    }
	}
	else if(!strcmp(splitParam[0],"quit")
		|| !strcmp(splitParam[0],"q"))
	{
	    arret_serveur(0);
	}
	else if(!strcmp(splitParam[0],"exe"))
	{
	    traitement_exe(splitParam,nbParam);
	}
	else
	{
	    printf("Le timer a écrit [%s]\n",buffer);
	}
    }
    return 0;
}


/**************************************************************************************
 *Fonction qui s'occupe de traiter les retours de script des clients lorsque ces       *
 *dernieres contactent le serveur sur le port 7002                                     *
 *                                                                                     *
 **************************************************************************************/

int traitement_reponse_script(char *secret)
{
    int num_row;
    char request[1024];
    char commande[1024];
    char cli_id[10];
    char script_id[10];
    MYSQL_RES *result=NULL;
    MYSQL_ROW row;
    con=connection_bdd();
    if(con==NULL)
    {
	syslog(LOG_ERR,"La connection a la bdd a échoué");
	return -1;
    }
    sprintf(request,"SELECT * FROM `DEMANDE_SCRIPT` WHERE `dem_secure`=\'%s\';",secret);
    if(mysql_query(con,request))
    {
	syslog(LOG_ERR,"%s\n",mysql_error(con));
	mysql_close(con);
	return -1;
    }
    result=mysql_store_result(con);
    if(result==NULL)
    {
	syslog(LOG_ERR,"%s\n",mysql_error(con));
	mysql_close(con);
	return -1;
    }
    num_row=mysql_num_rows(result);
    if(num_row!=1)
    {
	if(num_row==0)
	{
	    syslog(LOG_ERR,"Aucune demande de script ne porte ce secret");
	}
	else
	{
	    syslog(LOG_ERR,"Plus d'un demande de script porte ce secret");
	}
	mysql_free_result(result);
	mysql_close(con);
	return -1;
    }
    row=mysql_fetch_row(result);
    sprintf(cli_id,"%s",row[2]);
    sprintf(script_id,"%s",row[3]);
    /*printf("---------DEMANDE-SCRIPT-------------------------------------\n");
      printf("|%3s|%4s|%3s|%2s|%40s|\n",row[0],row[1],cli_id,script_id,row[4]);
      printf("------------------------------------------------------------\n");*/
    mysql_free_result(result);
    sprintf(request,"SELECT * FROM `SCRIPT` WHERE `script_id`=\'%s\';",script_id);
    if(mysql_query(con,request))
    {
	syslog(LOG_ERR,"%s\n",mysql_error(con));
	mysql_close(con);
	return -1;
    }
    result=mysql_store_result(con);
    if(result==NULL)
    {
	syslog(LOG_ERR,"%s\n",mysql_error(con));
	mysql_close(con);
	return -1;
    }
    num_row=mysql_num_rows(result);
    if(num_row!=1)
    {
	if(num_row==0)
	{
	    syslog(LOG_ERR,"Aucune script ne porte cet id");
	}
	else
	{
	    syslog(LOG_ERR,"Plus d'un script porte cet id");
	}
	mysql_free_result(result);
	mysql_close(con);
	return -1;
    }
    row=mysql_fetch_row(result);
    /*printf("---------SCRIPT-------------------------------------------------------------\n");
      printf("|%3s|%15s|%33s|%20s|\n",row[0],row[1],row[2],row[3]);
      printf("----------------------------------------------------------------------------\n");*/
    sprintf(commande,"./script/%s.py retour_script/%s %s\n",row[1],secret,cli_id);
    system(commande);
    printf("La réponse du script a été traité\n");
    mysql_free_result(result);
    mysql_close(con);
    return 0;
}

/**************************************************************************************
 *Ferme toutes les socket en ecoute                                                    *
 *                                                                                     *
 *                                                                                     *
 **************************************************************************************/

int fermeture_socket_ecoute()
{
    close(sock_in_1.sock);
    close(sock_in_2.sock);
    close(sock_web.sock);
    close(sock_in_unix.sock);
    return 0;
}

/**************************************************************************************
 *Teste aupres de la bdd si le client est enregistré                                   *
 *champs client_registered 1 => enregistre                                             *
 *                         0 => non                                                    *
 **************************************************************************************/

int is_client_registered(char *id_client)
{
    int num_row;
    char request[1024];
    MYSQL_RES *result=NULL;
    MYSQL_ROW row;
    con=connection_bdd();
    if(con==NULL)
    {
	syslog(LOG_ERR,"La connection a la bdd a échoué");
	return -1;
    }
    sprintf(request,"SELECT `cli_registered` FROM `CLIENTS` WHERE `cli_id`=\'%s\';",id_client);
    if(mysql_query(con,request))
    {
	syslog(LOG_ERR,"%s\n",mysql_error(con));
	mysql_close(con);
	return -1;
    }
    result=mysql_store_result(con);
    if(result==NULL)
    {
	syslog(LOG_ERR,"%s\n",mysql_error(con));
	mysql_close(con);
	return -1;
    }
    num_row=mysql_num_rows(result);
    if(num_row!=1)
    {
	if(num_row==0)
	{
	    syslog(LOG_ERR,"Aucune client ne possède cet ID");
	}
	else
	{
	    syslog(LOG_ERR,"Plus d'un client possède cet ID");
	}
	mysql_free_result(result);
	mysql_close(con);
	return -1;
    }
    row=mysql_fetch_row(result);
    if(!strcmp(row[0],"0"))
    {
	mysql_free_result(result);
	mysql_close(con);
	return -1;
    }
    else
    {
	mysql_free_result(result);
	mysql_close(con);
	return 0;
    }
}

/**************************************************************************************
 *Corp du cron qui a l'heure actuelle ne fait ... rien                                 *
 *                                                                                     *
 *                                                                                     *
 **************************************************************************************/

void* thread_timer()
{
    //ssize_t nb_bytes;
    Socket_un sock;
    init_connect_unix(&sock,"internal");
    //char buffer[1024];
    while(1)
    {    
	/*memset(buffer,0,1024);
	  sprintf(buffer,"exe 8 23");
	  nb_bytes=write(sock.sock,buffer,strlen(buffer));
	  if(nb_bytes==-1)
	  {
	  syslog(LOG_ERR,"%s",strerror(errno));
	  }*/
	sleep(5);
    }    
    return NULL;
}

/**************************************************************************************
 *Fonction qui va creer et lancer le thread timer (cron-like)                          *
 *                                                                                     *
 *                                                                                     *
 **************************************************************************************/

pthread_t lancement_thread_timer()
{
    Socket_un internal_listean;
    if(init_listean_unix(&internal_listean,"internal")>0)
    {
	syslog(LOG_ERR,"internal_listean %s",strerror(errno));
	return -1;
    }
    pthread_t thread;
    pthread_create(&thread,NULL,&thread_timer,NULL);
    init_accept_unix(&internal,internal_listean);
    close(internal_listean.sock);
    return thread;
}

/**************************************************************************************
 *Corps des thread d'assassinat des fils créés par fork                                *
 *                                                                                     *
 *                                                                                     *
 **************************************************************************************/

void* thread_kill(void *arg)
{
    pid_t *pid=arg;
    //pid_t r_pid;
    //r_pid=waitpid(*pid,NULL,0);
    waitpid(*pid,NULL,0);
    //printf("Un processus forké est mort [%d] (%d)\n",r_pid,*pid);
    free(pid);
    return NULL;
}

/**************************************************************************************
 *Lancement des thread d'assassinat des fils créés par fork                            *
 *                                                                                     *
 *                                                                                     *
 **************************************************************************************/

int lancement_thread_kill(pid_t pid)
{
    pid_t *pid_to_thread=NULL;
    pid_to_thread=malloc(sizeof(pid_t));
    *pid_to_thread=pid;
    pthread_t thread;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
    pthread_create(&thread,&attr,&thread_kill,(void*)pid_to_thread);
    pthread_attr_destroy(&attr);
    return 0;
}

/**************************************************************************************
 *Gestion des commandes passées par une socket unix provenenant d'un autre programme   *
 *pour permettre de communiquer avec le demon (non finit)                              *
 *                                                                                     *
 **************************************************************************************/

int traitement_entree_unix()
{
    ssize_t nb_bytes;
    char buffer[100];
    pid_t pid;
    if(init_accept_unix(&sock_out_unix,sock_in_unix))
    {
	return -1;
    }
    pid=fork();
    if(pid==-1)
    {
	syslog(LOG_ERR,"fork : failed in traitement_entree_unix");
	close(sock_out_unix.sock);
	return -1;
    }
    else if(pid==0)
    {
	fermeture_socket_ecoute();
	do
	{
	    nb_bytes=read(sock_out_unix.sock,buffer,100);
	    if(nb_bytes>0)
	    {
		buffer[nb_bytes]=0;
		printf("[%d] Le client a écrit: %s\n",(int)nb_bytes,buffer);
	    }
	}while(nb_bytes>0);
	close(sock_out_unix.sock);
	exit(0);
    }
    else
    {
	lancement_thread_kill(pid);
	close(sock_out_unix.sock);
	return 0;
    }
}

/**************************************************************************************
 *Fonction qui va genere un token alphanumérique de 40 caractères qui va               *
 *servir dans l'authentification dans les communications client-server                 *
 *                                                                                     *
 **************************************************************************************/

void generation_secret(char *secret,int taille)
{
    int i;
    char c;
    srand(time(NULL));
    for(i=0;i<taille;i++)
    {
	do{c=rand()%('z'-'0')+'0';}
	while((c>=':' && c<='@')||(c>='[' && c<='`'));
	secret[i]=c;
    }
    secret[i]=0;
}

/**************************************************************************************
 *Fonction qui va envoyer les scripts aux clients                                      *
 *                                                                                     *
 *                                                                                     *
 **************************************************************************************/

int envoi_script(char *file)
{
    int i=0;
    ssize_t nb_bytes;
    FILE *script=NULL;
    char path[100];
    char buffer[10];
    char buf;
    EnvoiScript data_out;
    // TODO : clean_EnvoiScript(&data_out)
    memset(data_out.contents,0,SIZE_CONTENTS);
    sprintf(path,"script/%s",file);
    script=fopen(path,"rb");
    if(script==NULL)
    {
	syslog(LOG_ERR,"%s\n",strerror(errno));
	return -1;
    }
    sprintf(data_out.header,"envoi_script_file");
    data_out.end=0;
    buf=fgetc(script);
    while(buf!=EOF)
    {
	data_out.contents[i++]=buf;
	if(i==SIZE_CONTENTS)
	{
	    data_out.size=i;
	    nb_bytes=write(sock_out.sock,(void*)&data_out,sizeof(data_out));
	    if(nb_bytes==-1)
	    {
		syslog(LOG_ERR,"%s\n",strerror(errno));
		fclose(script);
		return -1;
	    }
	    i=0;
	    memset(data_out.contents,0,SIZE_CONTENTS);
	    nb_bytes=read(sock_out.sock,buffer,9);
	    if(nb_bytes==-1)
	    {
		syslog(LOG_ERR,"%s\n",strerror(errno));
		fclose(script);
		return -1;
	    }
	    if(strcmp(buffer,"ok"))
	    {
		syslog(LOG_ERR,"Le client n'a pas acquitte correctement le paquet recu");
		fclose(script);
		return -1;
	    }
	}
	buf=fgetc(script);
    }
    data_out.end=1;
    data_out.size=i;
    nb_bytes=write(sock_out.sock,(void*)&data_out,sizeof(data_out));
    if(nb_bytes==-1)
    {
	syslog(LOG_ERR,"%s\n",strerror(errno));
	fclose(script);
	return -1;
    }
    fclose(script);
    return 0;
}

/**************************************************************************************
 * Fonction qui enregistre les token(secrets) dans la bdd                              *
 * dans la table DEMANDE_SCRIPT                                                        *
 *                                                                                     *
 **************************************************************************************/

int sauvegarde_secret(char *cli_id,char *secret,char *script_name)
{
    char request[1024];
    MYSQL_RES *result=NULL;
    MYSQL_ROW row;
    con=connection_bdd();
    if(con==NULL)
    {
	syslog(LOG_ERR,"La connection a la bdd a échoué");
	return -1;
    }
    sprintf(request,"SELECT `script_id` FROM `SCRIPT` WHERE `script_name`=\'%s\';",script_name);
    if(mysql_query(con,request))
    {
	syslog(LOG_ERR,"%s",mysql_error(con));
	BUG(buffer_info);
	syslog(LOG_INFO,"%s",buffer_info);
	mysql_close(con);
	return -1;
    }
    result=mysql_store_result(con);
    if(result==NULL)
    {
	syslog(LOG_ERR,"%s\n",mysql_error(con));
	mysql_close(con);
	return -1;
    }
    row=mysql_fetch_row(result);
    sprintf(request,"INSERT INTO DEMANDE_SCRIPT (cli_id,dem_secure,script_id) VALUES (\'%s\',\'%s\',\'%s\')",
	    cli_id,secret,row[0]);
    mysql_free_result(result);
    if(mysql_query(con,request))
    {
	syslog(LOG_ERR,"%s\n",mysql_error(con));
	mysql_close(con);
	return -1;
    }
    mysql_close(con);
    return 0;
}

/**************************************************************************************
 *Fork qui va s'occuper de demander a un client d'executer un script                   *
 *                                                                                     *
 *                                                                                     *
 **************************************************************************************/

int fork_exe(char *cli_id,char *name,char *hash,char *secret)
{
    ssize_t nb_bytes;
    Communication com_in,com_out;
    clean_Communication(&com_in);
    clean_Communication(&com_out);
    //Preparation du header du paquet
    sprintf(com_out.header,"execute");
    //Preparation des paramètres
    com_out.argc=3;
    sprintf(com_out.arg[0],"%s",name);
    sprintf(com_out.arg[1],"%s",hash);
    sprintf(com_out.arg[2],"%s",secret);
    //Envoi du paquet
    nb_bytes=write(sock_out.sock,(void*)&com_out,sizeof(com_out));
    if(nb_bytes==-1)
    {
	syslog(LOG_ERR,"%s\n",strerror(errno));
	return -1;
    }
    usleep(100);
    clean_Communication(&com_in);
    //Reception de la réponse
    nb_bytes=read(sock_out.sock,(void*)&com_in,sizeof(com_in));
    if(nb_bytes==-1)
    {
	syslog(LOG_ERR,"%s\n",strerror(errno));
	return -1;
    }
    //Traitement de la réponse
    if(!strcmp(com_in.header,"execute") 
	    && com_in.argc==1
	    && ( !strcmp(com_in.arg[0],"ok") 
		|| !strcmp(com_in.arg[0],"nofile")))
    {
	if(!strcmp(com_in.arg[0],"nofile"))
	{
	    if(envoi_script(name)==-1)
	    {
		syslog(LOG_ERR,"Une erreur est survenue lors de l'envoi d'un script à un client\n");
		return -1;
	    }
	}
	if(sauvegarde_secret(cli_id,secret,name)==-1)
	{
	    return -1;
	}
	return 0;
    }
    else
    {
	return -1;
    }
}

/**************************************************************************************
 *Fonction qui interroge la bdd pour recuperer l'ip d'un client dont l'id est passe    *
 *en argument                                                                          *
 *                                                                                     *
 **************************************************************************************/

int recuperation_ip_client(char *id_client,char *ip_client)
{
    con=connection_bdd();
    if(con==NULL)
    {
	return -1;
    }
    int num_row;
    char request[1024];
    MYSQL_ROW row;
    MYSQL_RES *result=NULL;
    sprintf(request,"SELECT `cli_ip` FROM `CLIENTS` WHERE `cli_id` = \'%s\' ;",id_client);
    if(mysql_query(con,request))
    {
	syslog(LOG_ERR,"%s\n",mysql_error(con));
	mysql_close(con);
	return -1;
    }
    result=mysql_store_result(con);
    if(result==NULL)
    {
	syslog(LOG_ERR,"%s\n",mysql_error(con));
	mysql_close(con);
	return -1;
    }
    num_row=mysql_num_rows(result);
    if(num_row!=1)
    {
	if(num_row==0)
	{
	    syslog(LOG_INFO,"Le client n'existe pas\n");
	}
	else
	{
	    syslog(LOG_ERR,"Il y a plusieurs client ayant le même id\n");
	}
	mysql_free_result(result);
	mysql_close(con);
	return -1;
    }
    else
    {
	//printf("Le client a été trouvé\n");
	row=mysql_fetch_row(result); 
	sprintf(ip_client,"%s",row[0]);
	mysql_free_result(result);
	mysql_close(con);
	return 0;
    }
}

/**************************************************************************************
 *Fonction qui interroge la bdd pour recuperer des informations à propos d'un script   *
 *dont l'id est passe en parametre                                                     *
 *On recupere le hash du script ainsi que son nom                                      *
 **************************************************************************************/

int recuperation_info_script(char *id_script,char *name,char *hash)
{
    con=connection_bdd();
    if(con==NULL)
    {
	return -1;
    }
    int num_row;
    char request[1024];
    MYSQL_ROW row;
    MYSQL_RES *result=NULL;
    sprintf(request,"SELECT `script_name`,`script_hash` FROM `SCRIPT` WHERE `script_id` = \'%s\' ;",id_script);
    if(mysql_query(con,request))
    {
	syslog(LOG_ERR,"%s\n",mysql_error(con));
	mysql_close(con);
	return -1;
    }
    result=mysql_store_result(con);
    if(result==NULL)
    {
	syslog(LOG_ERR,"%s\n",mysql_error(con));
	mysql_close(con);
	return -1;
    }
    num_row=mysql_num_rows(result);
    if(num_row!=1)
    {
	if(num_row==0)
	{
	    syslog(LOG_INFO,"Le script n'existe pas\n");
	}
	else
	{
	    syslog(LOG_ERR,"Il y a plusieurs client ayant le même id\n");
	}
	mysql_free_result(result);
	mysql_close(con);
	return -1;
    }
    else
    {
	//printf("Le script a été trouvé\n");
	row=mysql_fetch_row(result); 
	sprintf(name,"%s",row[0]);
	sprintf(hash,"%s",row[1]);
	mysql_free_result(result);
	mysql_close(con);
	return 0;
    }
}

/**************************************************************************************
 *fonction lance apres la commande exe <id_client> <id_script> ait ete envoye au server*
 *elle va recuperer toutes les informations preliminaires et lance la fonction         *
 *fork_exe                                                                             *
 **************************************************************************************/

int traitement_exe(char splitParam[20][100],int nbParam)
{
    int pid;
    char ip[100];
    char name[100];
    char hash[40];
    char secret[41];
    char id_client[5];
    char id_script[5];
    sprintf(id_client,"%s",splitParam[1]);
    sprintf(id_script,"%s",splitParam[2]);
    if(nbParam!=3)
    {
	printf("Nombre de paramètre incorrect\n");
	printf("Syntaxe : exe <id_client> <n°script>\n");
	return -1;
    }
    if(is_client_registered(id_client) == -1)
    {
	syslog(LOG_ERR,"Le client sélectionner n'est pas enregistré");
	return -1;
    }
    if(recuperation_ip_client(id_client,ip) < 0 )
    {
	printf("Erreur lors de la récupération de l'adresse ip du client\n");
	return -1;
    }
    if(recuperation_info_script(id_script,name,hash))
    {
	printf("Erreur lors de la récupération des infos du script\n");
	return -1;	
    }
    generation_secret(secret,40);
    if(!init_connect(&sock_out,ip,PORT_CLIENT_2))
    {
	printf("La connection avec le client a pu être établie\n");
	pid=fork();
	if(pid==-1)
	{
	    // SYSLOG
	    close(sock_out.sock);
	    return -1;
	}
	else if(pid==0)
	{
	    fermeture_socket_ecoute();
	    fork_exe(id_client,name,hash,secret);
	    close(sock_out.sock);
	    exit(0);
	}
	else
	{
	    lancement_thread_kill(pid);
	    close(sock_out.sock);
	    return 0;
	}
    }
    else
    {
	syslog(LOG_ERR,"La connection avec le client n'a pas pu être établie\n");
	return -1;
    }
}

/**************************************************************************************
 *Connection a la bdd                                                                  *
 *                                                                                     *
 *                                                                                     *
 **************************************************************************************/

MYSQL* connection_bdd()
{
    MYSQL *con=NULL;
    con=mysql_init(NULL);
    if(con==NULL)
    {
	// TODO :SYSLOG
	syslog(LOG_ERR,"%s\n",mysql_error(con));
	return NULL;
    }
    if((mysql_real_connect(con,"localhost",user_bdd,mdp_bdd,"PROJET_SECU",
		    0,NULL,0)) == NULL )
    {
	// TODO :SYSLOG
	syslog(LOG_ERR,"%s\n",mysql_error(con));
	return NULL;
    }
    return con;
}

/**************************************************************************************
 *Demarrage du server                                                                  *
 *Ouverture des socket et du systeme de log                                            *
 *                                                                                     *
 **************************************************************************************/

int initialisation_serveur()
{
    pid_t pid = getpid();
    printf("Demarrage serveur\n");
    printf("PID = [%d]\n",pid);
    openlog("server_audit",LOG_PID,LOG_LOCAL5);
    syslog(LOG_INFO,"starting");
    Initialisation_SSL();
    // A REFAIRE
    if(init_listean(&sock_in_1,PORT_SERV_1)>0)
    {
	syslog(LOG_ERR,"sock_in_1 %s",strerror(errno));
	return -1;
    }
    if(init_listean(&sock_in_2,PORT_SERV_2)>0)
    {
	syslog(LOG_ERR,"sock_in_2 %s",strerror(errno));
	return -1;
    }
    if(init_listean(&sock_web,PORT_WEB)>0)
    {
	syslog(LOG_ERR,"sock_web %s",strerror(errno));
	return -1;
    }
    if(init_listean_unix(&sock_in_unix,"essai")>0)
    {
	syslog(LOG_ERR,"sock_in_unix %s",strerror(errno));
	return -1;
    }
	recuperation_id_bdd();
    return 0;
}

/**************************************************************************************
 *Fermeture du server                                                                  *
 *                                                                                     *
 *                                                                                     *
 **************************************************************************************/

void arret_serveur(int retour)
{
    if(sock_in_1.sock > 0)
    {
	if(close(sock_in_1.sock) == -1)
	{
	    BUG(buffer_info);
	    syslog(LOG_INFO,"%s",buffer_info);
	    syslog(LOG_ERR,"sock_in_1 %s",strerror(errno));
	}
    }
    if(sock_in_2.sock > 0)
    {
	if(close(sock_in_2.sock) == -1)
	{
	    BUG(buffer_info);
	    syslog(LOG_INFO,"%s",buffer_info);
	    syslog(LOG_ERR,"sock_in_2 %s",strerror(errno));
	}
    }
    if(sock_web.sock > 0)
    {
	if(close(sock_web.sock) == -1)
	{
	    BUG(buffer_info);
	    syslog(LOG_INFO,"%s",buffer_info);
	    syslog(LOG_ERR,"sock_web %s",strerror(errno));
	}
    }
    if(sock_in_unix.sock > 0)
    {
	if(close(sock_in_unix.sock) == -1)
	{
	    BUG(buffer_info);
	    syslog(LOG_INFO,"%s",buffer_info);
	    syslog(LOG_ERR,"sock_in_unix %s",strerror(errno));
	}
    }
    syslog(LOG_INFO,"Arret du server");
    closelog();
    printf("\rArret serveur\n");
    exit(retour);
}

/**************************************************************************************
 *Verifie dans la bdd si le secret qu'un potentiel client existe et recupere de ce fait*
 *l'id de ce client                                                                    *
 *                                                                                     *
 **************************************************************************************/

int verification_secret(char *cli_id,char *secret)
{
    con=connection_bdd();
    if(con==NULL)
    {
	return -1;
    }
    char request[1024];
    int num_row;
    MYSQL_RES *result=NULL;
    MYSQL_ROW row;
    sprintf(request,"SELECT `cli_id` FROM `DEMANDE_SCRIPT` WHERE `dem_secure` = \'%s\';",secret);
    if(mysql_query(con,request))
    {
	syslog(LOG_ERR,"%s\n",mysql_error(con));
	mysql_close(con);
	return -1;
    }
    result = mysql_store_result(con);
    if(result==NULL)
    {
	syslog(LOG_ERR,"%s\n",mysql_error(con));
	mysql_close(con);
	return -1;
    }
    num_row = mysql_num_rows(result);
    if(num_row==1)
    {
	row=mysql_fetch_row(result);
	sprintf(cli_id,"%s",row[0]);
	mysql_free_result(result);
	mysql_close(con);
	return 0;
    }
    else if(num_row==0)
    {
	// ERREUR
	return -1;
    }
    else
    {
	// ERREUR
	return -1;
    }
}

/**************************************************************************************
 *Fonction qui va etre appellee lorsqu'un client se connecte sur le port 7001 pour     *
 *fournir le resultat d'un script. Elle va verifier les informations du client ,       *
 *recevoir les resultat et les traiter grace aux script python                         *
 **************************************************************************************/

int traitement_reception_script(Communication com_in)
{
    int i;
    FILE *retour_script=NULL;
    ssize_t nb_bytes;
    char secret[50];
    char cli_id[10];
    char path[100];
    char ok[10];
    sprintf(ok,"ok");
    if(!(!strcmp(com_in.header,"envoi_script")
		&& com_in.argc==2
		&& !strcmp(com_in.arg[0],"ask")
		&& (strcmp(com_in.arg[1],"") && strcmp(com_in.arg[1],"NULL"))))
    {
	syslog(LOG_ERR,"Un paquet provenant du client n'est pas conforme\n");
	return -1;
    }
    sprintf(secret,"%s",com_in.arg[1]);
    if(verification_secret(cli_id,secret)==-1)
    {
	syslog(LOG_ERR,"Le secret fournit par le client ne correspond pas\n");
	return -1;
    }
    Communication com_out;
    EnvoiScript data_in;
    clean_Communication(&com_out);
    data_in.end=0;
    memset(data_in.header,0,1024);
    memset(data_in.contents,0,1024);
    sprintf(com_out.header,"envoi_script");
    com_out.argc=1;
    sprintf(com_out.arg[0],"allow");
    nb_bytes=write(sock_out.sock,(void*)&com_out,sizeof(com_out));
    if(nb_bytes==-1)
    {
	syslog(LOG_ERR,"%s\n",strerror(errno));
	return -1;
    }
    sprintf(path,"retour_script/%s",secret);
    retour_script=fopen(path,"w");
    do
    {
	nb_bytes=read(sock_out.sock,(void*)&data_in,sizeof(data_in));
	if(nb_bytes==-1)
	{
	    syslog(LOG_ERR,"%s\n",strerror(errno));
	    return -1;
	}
	printf("%s",data_in.contents);
	for(i=0;i<data_in.size;i++)
	{
	    fputc(data_in.contents[i],retour_script);
	}
	nb_bytes=write(sock_out.sock,ok,strlen(ok));
	if(nb_bytes==-1)
	{
	    syslog(LOG_ERR,"%s\n",strerror(errno));
	    return -1;
	}
    }while(!data_in.end);
    fclose(retour_script);
    clean_Communication(&com_out);
    sprintf(com_out.header,"envoi_script");
    sprintf(com_out.arg[0],"end");
    com_out.argc=1;
    nb_bytes=write(sock_out.sock,(void*)&com_out,sizeof(com_out));
    if(nb_bytes==-1)
    {
	syslog(LOG_ERR,"%s\n",strerror(errno));
	return -1;
    }
    if(traitement_reponse_script(secret)==-1)
    {
	syslog(LOG_ERR,"Une erreur est survenue lors du traitement du retour du script\n");
	return -1;
    }
    return 0;
}

/**************************************************************************************
 *Fonction qui s'occupe de l'enregistrement d'un client lorsque celui ci le reclame    *
 *lorsque que ce dernier se connecte sur le port 7000                                  *
 *                                                                                     *
 **************************************************************************************/

int traitement_request_register(Communication com_in)
{
    if(com_in.argc!=2)
    {
	syslog(LOG_ERR,"erreur dans le nombre d'argument fournit par le client");// TODO
	return -1;
    }
    if(!strcmp(com_in.arg[1],"") || !strcmp(com_in.arg[1],"NULL"))
    {
	syslog(LOG_ERR,"erreur dans la version fournit par le client");// TODO
	return -1;
    }
    if(!strcmp(com_in.arg[0],"") || !strcmp(com_in.arg[0],"NULL"))
    {
	syslog(LOG_ERR,"erreur dans l'hostname fournit par le client");// TODO
	return -1;
    }
    con=connection_bdd();
    if(con==NULL)
    {
	return -1;
    }
    ssize_t nb_bytes;
    int num_row;;
    char request[1024];
    char ip_client[100];
    char hostname_client[100];
    char version_client[100];
    MYSQL_RES *result=NULL;
    Communication com_out;
    clean_Communication(&com_out);
    // INFORMATIONS CLIENT
    sprintf(ip_client,"%s",inet_ntoa(sock_out.sin.sin_addr));
    sprintf(hostname_client,"%s",com_in.arg[0]);
    sprintf(version_client,"%s",com_in.arg[1]);
    // INTERROGATION SQL
    sprintf(request,"SELECT * FROM CLIENTS WHERE cli_ip = \'%s\'",ip_client);
    // TODO : securiser injection SQL
    if(mysql_query(con,request))
    { 
	syslog(LOG_ERR,"ERREUR : %s\n",mysql_error(con));
	mysql_free_result(result);
	mysql_close(con);
	return -1;
    }
    result = mysql_store_result(con);
    if(result==NULL)
    {
	syslog(LOG_ERR,"ERREUR : %s\n",mysql_error(con));
	mysql_free_result(result);
	mysql_close(con);
	return -1;
    }
    num_row = mysql_num_rows(result);
    if(num_row!=0)
    {
	syslog(LOG_ERR,"il existe deja un client");// TODO
	mysql_free_result(result);
	mysql_close(con);
	return -1;
    }
    sprintf(com_out.header,"request_register_2");
    com_out.argc=1;
    sprintf(com_out.arg[0],"%s",HOSTNAME_SERVER);
    write(sock_out.sock,(void*)&com_out,sizeof(com_out));
    usleep(100);
    clean_Communication(&com_in);
    nb_bytes=lecturesock(sock_out,(void*)&com_in,sizeof(com_in));
    if(nb_bytes==-1)
    {
	syslog(LOG_ERR,"la connection avec le client a été interrompu");// TODO
	mysql_free_result(result);
	mysql_close(con);
	return -1;	
    }
    if(strcmp(com_in.header,"request_register_3"))
    {
	syslog(LOG_ERR,"le client n'a pas renvoye request_register_3");// TODO
	mysql_free_result(result);
	mysql_close(con);
	return -1;	
    }
    sprintf(request,"INSERT INTO CLIENTS (cli_ip,cli_hostname,cli_registered) VALUES (\'%s\',\'%s\',TRUE)",
	    ip_client,hostname_client);
    // TODO : securiser injection sql
    if(mysql_query(con,request))
    {
	syslog(LOG_ERR,"%s\n",mysql_error(con));
	mysql_free_result(result);
	mysql_close(con);
	return -1;	
    }
    mysql_free_result(result);
    mysql_close(con);
    return 0;
}

/**************************************************************************************
 *Fonction d'aiguillage des requetes arrivant sur le port 7000 du server               *
 *                                                                                     *
 *                                                                                     *
 **************************************************************************************/

int traitement_entree_1()
{
    ssize_t nb_bytes;
    int i;
    pid_t pid;
    Communication com_in;
    if(init_accept(&sock_out,sock_in_1))
    {
	syslog(LOG_ERR,"erreur init_accept dans traitement_entree_1");//TODO
	return -1;
    }
    pid=fork();
    if(pid==-1)
    {
	syslog(LOG_ERR,"erreur fork dans traitement_entree_1");//TODO
	close(sock_out.sock);
	return -1;
    }
    else if(pid==0)
    {
	fermeture_socket_ecoute();
	usleep(100);
	clean_Communication(&com_in);
	nb_bytes=read(sock_out.sock,(void*)&com_in,sizeof(com_in));
	if(nb_bytes==-1)
	{
	    syslog(LOG_ERR," read dans traitement_entree_1 : %s\n",strerror(errno));
	    close(sock_out.sock);
	    exit(-1);
	}
	if(!strcmp(com_in.header,"request_register"))
	{
	    if(traitement_request_register(com_in)==-1)
	    {
		close(sock_out.sock);
		syslog(LOG_ERR,"traitement_request_register(com_in) : failed\n");
		exit(-1);
	    }
	    close(sock_out.sock);
	    exit(0);
	}
	else
	{
	    printf("Le client a écrit: %s\n",com_in.header);
	    if(com_in.argc>0)
	    {
		printf("Ainsi que %d arguments\n",com_in.argc);
		for(i=0;i<com_in.argc;i++)
		{   
		    printf("arg[%d] = %s\n",i,com_in.arg[i]);
		}
	    }
	    close(sock_out.sock);
	    exit(0);
	}
    }
    lancement_thread_kill(pid);
    close(sock_out.sock);
    return 0;
}

/**************************************************************************************
 *Fonction d'aiguillage des requetes arrivant sur le port 7001 du server               *
 *                                                                                     *
 *                                                                                     *
 **************************************************************************************/

int traitement_entree_2()
{
    ssize_t nb_bytes;
    int i;
    pid_t pid;
    Communication com_in;
    if(init_accept(&sock_out,sock_in_2))
    {
	return -1;
    }
    pid=fork();
    if(pid==-1)
    {
	close(sock_out.sock);
	syslog(LOG_ERR,"une erreur est survenue lors du forking du processus pere");
	return -1;
    }
    else if(pid==0)
    {
	fermeture_socket_ecoute();
	close(sock_in_1.sock);
	close(sock_in_2.sock);
	close(sock_web.sock);
	usleep(100);
	clean_Communication(&com_in);
	nb_bytes=read(sock_out.sock,(void*)&com_in,sizeof(com_in));
	if(nb_bytes==-1)
	{
	    syslog(LOG_ERR," read(sock_out.sock,(void*)&com_in,sizeof(com_in)) : %s\n",strerror(errno));
	    close(sock_out.sock);
	    exit(-1);
	}
	if(!strcmp(com_in.header,"envoi_script"))
	{
	    if(traitement_reception_script(com_in)==-1)
	    {
		close(sock_out.sock);
		syslog(LOG_ERR,"traitement_reception_script(com_in) : failed\n");
		exit(-1);
	    }
	}
	else
	{
	    printf("Le client a écrit: %s\n",com_in.header);
	    if(com_in.argc>0)
	    {
		printf("Ainsi que %d arguments\n",com_in.argc);
		for(i=0;i<com_in.argc;i++)
		{
		    printf("arg[%d] = %s\n",i,com_in.arg[i]);
		}
	    }
	}
	close(sock_out.sock);
	exit(0);
    }
    lancement_thread_kill(pid);
    close(sock_out.sock);
    return 0;
}

/**************************************************************************************
 *Fonction d'aiguillage des requetes arrivant sur le port 7002 du server (web)         *
 *                                                                                     *
 *                                                                                     *
 **************************************************************************************/

int traitement_entree_web()
{
    char buffer[1024];
    int nbParam=0;
    char splitParam[20][100];
    ssize_t nb_bytes;
    Socket yolo;
    if(init_accept(&yolo,sock_web))
    {
	printf("ERREUR\n");
	return -1;
    }
    else
    {
	usleep(100);
	nb_bytes=lecturesock(yolo,buffer,sizeof(buffer));
	if(nb_bytes==-1)
	{
	    syslog(LOG_ERR," read(sock_out.sock,(void*)&com_in,sizeof(com_in)) : %s\n",strerror(errno));
	    close(yolo.sock);
	    exit(-1);
	}
	printf("du web : %s\n",buffer);
	nbParam=decoupageCommande(buffer," ",splitParam);
	if(nbParam>0)
	{
	    if(!strcmp(splitParam[0],"register"))
	    {
		traitement_register(splitParam[1]);
	    }
	    else if(!strcmp(splitParam[0],"exe"))
	    {
		traitement_exe(splitParam,nbParam);
	    }
	}
	close(yolo.sock);
	return 0;
    }
}

/**************************************************************************************
 *Analyse et aiguillage des requetes tapees au clavier                                 *
 *                                                                                     *
 *                                                                                     *
 **************************************************************************************/

int traitement_entree_clavier()
{
    int nbParam=0;
    char buffer[1024];
    char splitParam[20][100];
    lectureSTDIN(buffer);
    nbParam=decoupageCommande(buffer," ",splitParam);
    if(nbParam>0)
    {
	if(!strcmp(splitParam[0],"update"))
	{
	    traitement_update();
	}
	else if(!strcmp(splitParam[0],"register"))
	{
	    traitement_register(splitParam[1]);
	}
	else if(!strcmp(splitParam[0],"add"))
	{
	    traitement_add(splitParam,nbParam);
	    traitement_show();
	}
	else if(!strcmp(splitParam[0],"del")
		|| !strcmp(splitParam[0],"rm"))
	{
	    traitement_del(splitParam,nbParam);
	    traitement_show();
	}
	else if(!strcmp(splitParam[0],"show")
		|| !strcmp(splitParam[0],"ls"))
	{
	    if(nbParam>1 && (!strcmp(splitParam[1],"script")
			|| !strcmp(splitParam[1],"scripts")
			|| !strcmp(splitParam[1],"sh")))
	    {
		traitement_show_script();
	    }
	    else
	    {
		traitement_show();
	    }
	}
	else if(!strcmp(splitParam[0],"quit")
		|| !strcmp(splitParam[0],"q"))
	{
	    arret_serveur(0);
	}
	else if(!strcmp(splitParam[0],"exe"))
	{
	    traitement_exe(splitParam,nbParam);
	}
	else
	{
	    printf("L'utilisateur a écrit [%s]\n",buffer);
	}
    }
    return 0;
}

/**************************************************************************************
 *Enregistrement de tous les clients non enregistres                                   *
 *                                                                                     *
 *                                                                                     *
 **************************************************************************************/

int traitement_update()
{
    con=connection_bdd();
    // TODO ERR
    char request[1024];
    int num_row;
    int pid;
    int retour=0;
    MYSQL_ROW row;
    MYSQL_RES *result=NULL;
    sprintf(request,
	    "SELECT * FROM CLIENTS WHERE `cli_registered`=0 ;");
    // TODO : securiser injection SQL
    if(mysql_query(con,request))
    { 
	syslog(LOG_ERR,"ERREUR SQL: %s\n",mysql_error(con));
	mysql_close(con);
	return 1001;// TODO -1
    }
    result = mysql_store_result(con);
    if(result==NULL)
    {
	syslog(LOG_ERR,"ERREUR SQL : %s\n",mysql_error(con));
	mysql_close(con);
	return 1002;	
    }
    num_row = mysql_num_rows(result);
    if(num_row)
    {
	while((row=mysql_fetch_row(result)))
	{
	    retour=init_connect(&sock_out,row[1],PORT_CLIENT_1);
	    if(!retour)
	    {
		pid=fork();
		if(pid==0)
		{
		    fermeture_socket_ecoute();
		    enregistrement_client(row[0]);
		    mysql_free_result(result);
		    mysql_close(con);
		    close(sock_out.sock);
		    exit(0);
		}
		lancement_thread_kill(pid);
		close(sock_out.sock);
	    }
	}
    }
    mysql_free_result(result);
    mysql_close(con);
    return 0; 
}

/**************************************************************************************
 *Enregistrement du client dont l'id est passe en parametre                            *
 *                                                                                     *
 *                                                                                     *
 **************************************************************************************/

int enregistrement_client(char *id_client)
{
    con=connection_bdd();
    char request[1024];
    Communication com_in,com_out;
    clean_Communication(&com_in);
    clean_Communication(&com_out);
    sprintf(com_out.header,"Request_Add");
    com_out.argc=1;
    sprintf(com_out.arg[0],HOSTNAME_SERVER);
    write(sock_out.sock,(void*)&com_out,sizeof(com_out));
    usleep(100);
    lecturesock(sock_out,(void*)&com_in,sizeof(com_in));
    if(!strcmp(com_in.header,"Request_Add_2"))
    {
	clean_Communication(&com_out);
	sprintf(com_out.header,"Request_Add_3");
	com_out.argc=1;
	sprintf(com_out.arg[0],"ok");
	write(sock_out.sock,(void*)&com_out,sizeof(com_out));
	sprintf(request,
		"UPDATE CLIENTS SET cli_hostname=\'%s\',cli_registered=1 WHERE cli_id = %s",
		com_in.arg[0],id_client);
	if(mysql_query(con,request))
	{
	    syslog(LOG_ERR,"ERREUR SQL : %s\n",mysql_error(con));
	    return 1001;
	}
	return 0;
    }
    mysql_close(con);
    return 1;
}

/**************************************************************************************
 *ajout d'un client a la bdd qui devra ensuite etre enregistre                         *
 *                                                                                     *
 *                                                                                     *
 **************************************************************************************/

int traitement_add(char splitParam[20][100],int nbParam)
{
    int retourFonction=0;
    if(nbParam==2)
    {
	retourFonction=add_client(splitParam[1]);
	if(retourFonction>1000)
	{
	    printf("ERREUR : Connection bdd perdue : relancer le serveur");
	    continu=0;
	    return 1;
	}
	else if(retourFonction)
	{
	    return 2;
	}
    }
    else
    {
	printf("WARNING : la fonction add() a seulement besoin ");
	printf("d'une adresse IP comme argument\n");
    }
    return 0;
}

/**************************************************************************************
 *ajout d'un client a la bdd qui devra ensuite etre enregistre                         *
 *                                                                                     *
 *                                                                                     *
 **************************************************************************************/

int add_client(char *buffer)
{
    con=connection_bdd();
    char request[1024];
    int num_row;;
    MYSQL_RES *result=NULL;
    if(buffer==NULL)
    {
	printf("ERREUR : add_client()[1]:buffer==NULL\n");
	mysql_free_result(result);
	mysql_close(con);
	return 1;
    }
    if(!isBufferAnIp(buffer))
    {
	printf("ERREUR : Le paramètre envoyé n'est pas une adresse IPv4 valide\n");
	mysql_free_result(result);
	mysql_close(con);
	return 2;
    }
    sprintf(request,"SELECT * FROM CLIENTS WHERE cli_ip = \'%s\'",buffer);
    // TODO : securiser injection SQL
    if(mysql_query(con,request))
    { 
	printf("ERREUR : %s\n",mysql_error(con));
	mysql_free_result(result);
	mysql_close(con);
	return 1001;
    }
    result = mysql_store_result(con);
    if(result==NULL)
    {
	printf("ERREUR : %s\n",mysql_error(con));
	mysql_free_result(result);
	mysql_close(con);
	return 1002;	
    }
    num_row = mysql_num_rows(result);
    if(num_row==1)
    {
	printf("WARNING : Il y a déjà une entrée sql ");
	printf("correspondante à cette adresse ip\n");
	mysql_free_result(result);
	mysql_close(con);
	return 3;
    }
    else if(num_row==0)
    {
	sprintf(request,"INSERT INTO CLIENTS (cli_ip) VALUES (\'%s\')",buffer);
	// TODO : securiser injection sql
	if(mysql_query(con,request))
	{
	    fprintf(stderr,"%s\n",mysql_error(con));
	    mysql_free_result(result);
	    mysql_close(con);
	    return 1003;
	}
	printf("Une nouvelle entrée à été ajouter à sql\n");
	mysql_free_result(result);
	mysql_close(con);
	return 0;
    }
    else
    {
	printf("WARNING : dans la bdd,");
	printf("<0 || >1 entrée possédant ");
	printf("la même adresse IP\n");
	mysql_free_result(result);
	mysql_close(con);
	return 4;
    }
}

/**************************************************************************************
 *lancement de la fonction de suppression d'un client                                  *
 *                                                                                     *
 *                                                                                     *
 **************************************************************************************/

int traitement_del(char splitParam[20][100],int nbParam)
{
    int retourFonction=0;
    if(nbParam==2)
    {
	retourFonction=del_client(splitParam[1]);
	if(retourFonction>1000)
	{
	    printf("ERREUR : Connection bdd perdue : relancer le serveur");
	    continu=0;
	    return 1;
	}
	else if(retourFonction)
	{
	    return 2;
	}
    }
    else
    {
	printf("WARNING : la fonction del() a seulement besoin ");
	printf("d'une adresse IP comme argument\n");
    }
    return 0;
}

/**************************************************************************************
 *suppression d'un client de la bdd                                                    *
 *                                                                                     *
 *                                                                                     *
 **************************************************************************************/

int del_client(char *buffer)
{
    con=connection_bdd();
    char request[1024];
    int num_row;;
    MYSQL_RES *result=NULL;
    if(buffer==NULL)
    {
	syslog(LOG_ERR,"ERREUR : del_client()[1]:buffer==NULL");
	mysql_free_result(result);
	mysql_close(con);
	return -1;
    }
    sprintf(request,"SELECT * FROM CLIENTS WHERE cli_id = \'%s\'",buffer);
    // TODO : securiser injection SQL
    if(mysql_query(con,request))
    { 
	syslog(LOG_ERR,"ERREUR : %s",mysql_error(con));
	mysql_free_result(result);
	mysql_close(con);
	return -1;
    }
    result = mysql_store_result(con);
    if(result==NULL)
    {
	syslog(LOG_ERR,"ERREUR : %s",mysql_error(con));
	mysql_free_result(result);
	mysql_close(con);
	return -1;	
    }
    num_row = mysql_num_rows(result);
    if(num_row==0)
    {
	printf("WARNING : Il n'y a aucune une entrée sql ");
	printf("correspondante à ce numero de client\n");
	mysql_free_result(result);
	mysql_close(con);
	return -1;
    }
    else if(num_row==1)
    {
	sprintf(request,"DELETE FROM CLIENTS WHERE cli_id = \'%s\'",buffer);
	// TODO : securiser injection sql
	if(mysql_query(con,request))
	{
	    syslog(LOG_ERR,"%s\n",mysql_error(con));
	    mysql_free_result(result);
	    mysql_close(con);
	    return -1;
	}
	printf("Un client à été supprimé de la bdd\n");
	mysql_free_result(result);
	mysql_close(con);
	return 0;
    }
    else
    {
	printf("WARNING : dans la bdd,");
	printf("<0 || >1 entrée possédant ");
	printf("la même adresse IP\n");
	mysql_free_result(result);
	mysql_close(con);
	return -1;
    }
}

/**************************************************************************************
 *Affichage des differents clients presents dans la bdd                                *
 *                                                                                     *
 *                                                                                     *
 **************************************************************************************/

int traitement_show()
{
    con=connection_bdd();
    if(con==NULL)
    {
	return -1;
    }
    int i;
    char request[1024];
    int num_row,num_fields;
    MYSQL_RES *result=NULL;
    MYSQL_ROW row;
    sprintf(request,"SELECT * FROM CLIENTS");
    if(mysql_query(con,request))
    {
	syslog(LOG_ERR,"%s\n",mysql_error(con));
	mysql_close(con);
	return -1;
    }
    result = mysql_store_result(con);
    if(result==NULL)
    {
	syslog(LOG_ERR,"%s\n",mysql_error(con));
	mysql_close(con);
	return -1;
    }
    num_row = mysql_num_rows(result);
    num_fields = mysql_num_fields(result);
    printf("Il y a %d entrée dans la table CLIENTS\n",num_row);
    if(num_row)
    {
	for(i=0;i<((5*3+1)+(2+15+20+10+12));i++)
	{
	    printf("-");
	}
	printf("\n");
	while((row=mysql_fetch_row(result)))
	{
	    for(i=0;i<num_fields;i++)
	    {
		switch(i)
		{
		    case 0:
			printf("| %2s ",row[i]?row[i]:"NULL");
			break;
		    case 1:
			printf("| %15s ",row[i]?row[i]:"NULL");
			break;
		    case 2:
			printf("| %20s ",row[i]?row[i]:"NULL");
			break;
		    case 3:
			printf("| %10s ",row[i]?row[i]:"NULL");
			break;
		    case 4:
			if(!strcmp(row[i],"0"))
			{
			    printf("| UNREGISTERED ");
			}
			else
			{
			    printf("|  REGISTERED  ");
			}
			break;
		    default:
			break;
		}
	    }
	    printf("|\n");
	}
	for(i=0;i<((5*3+1)+(2+15+20+10+12));i++)
	{
	    printf("-");
	}
	printf("\n");
    }
    mysql_free_result(result);
    mysql_close(con);
    return 0;
}

/**************************************************************************************
 *Affichage des differents scripts disponibles                                          *
 *                                                                                     *
 *                                                                                     *
 **************************************************************************************/

int traitement_show_script()
{
    con=connection_bdd();
    if(con==NULL)
    {
	return -1;
    }
    int i;
    char request[1024];
    int num_row,num_fields;
    MYSQL_RES *result=NULL;
    MYSQL_ROW row;
    sprintf(request,"SELECT `script_id`,`script_descr` FROM SCRIPT");
    // TODO : securiser injection SQL
    if(mysql_query(con,request))
    { 
	syslog(LOG_ERR,"%s\n",mysql_error(con));
	mysql_close(con);
	return -1;
    }
    result = mysql_store_result(con);
    if(result==NULL)
    {
	syslog(LOG_ERR,"%s\n",mysql_error(con));
	mysql_close(con);
	return -1;
    }
    num_row = mysql_num_rows(result);
    num_fields = mysql_num_fields(result);
    printf("Il y a %d entrée dans la table SCRIPT\n",num_row);
    if(num_row)
    {
	for(i=0;i<((2*3+1)+(2+50));i++)
	{
	    printf("-");
	}
	printf("\n");
	while((row=mysql_fetch_row(result)))
	{
	    for(i=0;i<num_fields;i++)
	    {
		switch(i)
		{
		    case 0:
			printf("| %2s ",row[i]?row[i]:"NULL");
			break;
		    case 1:
			printf("| %50s ",row[i]?row[i]:"NULL");
			break;
		    default:
			break;
		}
	    }
	    printf("|\n");
	}
	for(i=0;i<((2*3+1)+(2+50));i++)
	{
	    printf("-");
	}
	printf("\n");
    }
    mysql_free_result(result);
    mysql_close(con);
    return 0;
}

/**************************************************************************************
 *interception des signaux SIGINT et modification de leur comportement                 *
 *                                                                                     *
 *                                                                                     *
 **************************************************************************************/

sigset_t set_masque_SIGINT()
{
    sigset_t sigmask, empty_mask;
    struct sigaction sa;
    sigemptyset(&sigmask);
    sigaddset(&sigmask,SIGINT);
    if(sigprocmask(SIG_BLOCK,&sigmask,NULL) == -1)
    {	
	syslog(LOG_ERR,"%s",strerror(errno));
	arret_serveur(-1);
    }
    sa.sa_flags=0;
    sa.sa_handler=child_sig_handler;
    sigemptyset(&sa.sa_mask);
    if(sigaction(SIGINT,&sa,NULL)==-1)
    {
	syslog(LOG_ERR,"%s",strerror(errno));
	arret_serveur(-1);
    }
    sigemptyset(&empty_mask);
    return empty_mask; 
}

/**************************************************************************************
 *Modification des comportement SIGINT                                                 *
 *^C => arret serveur propre                                                           *
 *                                                                                     *
 **************************************************************************************/

void child_sig_handler(int signum)
{
    if(signum==SIGINT)
    {
	arret_serveur(0);
    }
}
// --- FONCTIONS -------------------------------------------------------------------- //
