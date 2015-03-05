/*
 * DESCRIPTION DU FICHIER 
 * Name : client.c
 */

// --- INCLUDE ------------------------------------------------------------------ //

#include "client.h"

// --- INCLUDE ------------------------------------------------------------------ //

// --- ZONE TEST ---------------------------------------------------------------- //
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
int MD5_Hash(char *hash,ssize_t size_hash,char *path)
{
    int i;
    char buffer[1024];
    unsigned char buffer2[16];
    char tmp[10];
    memset(hash,0,size_hash);
    memset(buffer2,0,16);
    MD5_CTX c;
    FILE *file=NULL;
    file=fopen(path,"r");
    if(file==NULL)
    {
	syslog(LOG_ERR,"le fichier %s n'existe pas : hashage impossible\n",path);
	return -1;
    }
    MD5_Init(&c);
    while(fgets(buffer,1024,file)!=NULL)
    {
	MD5_Update(&c,buffer,(unsigned long)strlen(buffer));
    }
    MD5_Final(buffer2,&c);
    for(i=0;i<16;i++)
    {
	sprintf(tmp,"%02x",buffer2[i]);
	strcat(hash,tmp);
    }
    fclose(file);
    return 0;
}
// --- ZONE TEST ---------------------------------------------------------------- //

// --- FONCTION MAIN ------------------------------------------------------------ //

int main (int argc, char* argv[]) 
{
	if (argc){}
	char dossier[100];
	dossier_lancement(argv[0], dossier);
	verifier_dossier_lancement(dossier);
    if(initialisation_client() == -1)
    {
		arret_client(-1);
    }
    fd_set rdfs;
    sigset_t empty_mask = set_masque_SIGINT();//TESTING
    while(continu)
    {
		FD_ZERO(&rdfs);
		FD_SET(STDIN_FILENO,&rdfs);
		FD_SET(sock_in_1.sock,&rdfs);
		FD_SET(sock_in_2.sock,&rdfs);
		if(pselect(sock_in_2.sock+1,&rdfs,NULL,NULL,NULL,&empty_mask)==-1)
		{
			if(errno != EINTR)
			{
			syslog(LOG_ERR,"%s\n",strerror(errno));
			arret_client(-1);
			}
		}
		if(continu)
		{
			if(FD_ISSET(STDIN_FILENO,&rdfs))
			{
			traitement_entree_clavier();
			}
			else if(FD_ISSET(sock_in_1.sock,&rdfs))
			{
			connection_client_1();
			}
			else if(FD_ISSET(sock_in_2.sock,&rdfs))
			{
			connection_client_2();
			}
		}
    }
    return 0;
}

// ------ FONCTION MAIN ------------------------------------------------------------ //

// ------ FONCTIONS ---------------------------------------------------------------- //

/**************************************************************************************
 *corps du thread qui a pour but de tuer les fork créés par le processus               *
 *principale                                                                           *
 *                                                                                     *
 **************************************************************************************/

void* thread_kill(void *arg)
{
    pid_t *pid=arg;
    pid_t r_pid;
    r_pid=waitpid(*pid,NULL,0);
    printf("Un processus forké est mort [%d] (%d)\n",r_pid,*pid);
    free(pid);
    return NULL;
}

/**************************************************************************************
 *lancement des thread qui ont pour but de tuer les fork créés par le processus        *
 *pere                                                                                 *
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
 *Fonction d'initialisation du client                                                  *
 *ouverture des socket d'ecoute etc                                                    *
 *                                                                                     *
 **************************************************************************************/

int initialisation_client()
{
    pid_t pid = getpid(); 
    printf("Demarrage client\n");
    printf("PID = [%d]\n",pid);
    openlog("client_audit",LOG_PID,LOG_LOCAL5);
    syslog(LOG_INFO,"starting");
    if(init_listean(&sock_in_1,PORT_CLIENT_1) > 0 )
    {
	syslog(LOG_ERR,"sock_in_1 %s",strerror(errno));
	return -1;
    }
    if(init_listean(&sock_in_2,PORT_CLIENT_2) > 0 )
    {
	syslog(LOG_ERR,"sock_in_2 %s",strerror(errno));
	return -1;
    }
    lecture_fichier_conf();
    return 0;
}

/**************************************************************************************
 *Fonction d'arret du client                                                           *
 *fermeture des socket d'ecoute etc ...                                                *
 *                                                                                     *
 **************************************************************************************/

int arret_client(int retour)
{
    maj_conf();
    syslog(LOG_INFO,"Fermeture des sockets encore ouverte");
    if(sock_in_1.sock != 0)
    {
	if(close(sock_in_1.sock) == -1)
	{
	    syslog(LOG_ERR,"sock_in_1 %s",strerror(errno));
	}
    }
    if(sock_in_2.sock != 0)
    {
	if(close(sock_in_2.sock) == -1)
	{
	    syslog(LOG_ERR,"sock_in_2 %s",strerror(errno));
	}
    }
    if(sock_out.sock != 0)
    {
	if(close(sock_out.sock) == -1)
	{
	    syslog(LOG_ERR,"sock_out %s",strerror(errno));
	}
    }
    syslog(LOG_INFO,"Arret du client");
    closelog();
    printf("\rArret client\n");
    exit(retour);	
}

/**************************************************************************************
 *fonction procedant a l'enregistrement aupres du serveur                              *
 *                                                                                     *
 *                                                                                     *
 **************************************************************************************/

int traitement_register()
{
    ssize_t nb_bytes;
    Communication com_in,com_out;
    clean_Communication(&com_in);
    clean_Communication(&com_out);
    if(!strcmp(server_ip,"NULL"))
    {
	syslog(LOG_INFO,"register impossible, aucune adresse ip renseigne");
	return -1;
    }
    if(strcmp(server_ip,"NULL") && strcmp(server_hostname,"NULL"))
    {
	syslog(LOG_INFO,"register impossible, client déja enregistré aupres d'un serveur");
	return -1;
    }
    if(init_connect(&sock_out,server_ip,PORT_SERV_1))
    {
	syslog(LOG_ERR,"erreur lors de la connection avec le serveur");//TODO
	return -1;
    }
    sprintf(com_out.header,"request_register");
    com_out.argc=2;
    sprintf(com_out.arg[0],"%s",HOSTNAME);
    sprintf(com_out.arg[1],"%s",VERSION);
    write(sock_out.sock,(void*)&com_out,sizeof(com_out));
    usleep(100);
    nb_bytes=lecturesock(sock_out,(void*)&com_in,sizeof(com_in));
    if(nb_bytes==-1)
    {
	syslog(LOG_ERR,"%s",strerror(errno));//TODO
	close(sock_out.sock);
	return -1;
    }
    if(strcmp(com_in.header,"request_register_2"))
    {
	syslog(LOG_ERR,"le serveur n'a pas renvoye request_register_2");//TODO
	close(sock_out.sock);
	return -1;
    }
    if(com_in.argc!=1)
    {
	syslog(LOG_ERR,"le nombre d'argument fournit par le serveur n'est pas valide");//TODO
	close(sock_out.sock);
	return -1;
    }
    if(!strcmp(com_in.arg[0],"") || !strcmp(com_in.arg[0],"NULL"))
    {
	syslog(LOG_ERR,"l'hostname fournit par le serveur n'est pas valide");//TODO
	close(sock_out.sock);
	return -1;
    }
    clean_Communication(&com_out);
    sprintf(com_out.header,"request_register_3");
    write(sock_out.sock,(void*)&com_out,sizeof(com_out));
    sprintf(server_ip,"%s",inet_ntoa(sock_out.sin.sin_addr));
    sprintf(server_hostname,"%s",com_in.arg[0]);
    close(sock_out.sock);
    return 0;
}

/**************************************************************************************
 *Met à jour le fichier de conf avec les variables globale server_ip et server_hostname*
 *                                                                                     *
 *                                                                                     *
 **************************************************************************************/

int maj_conf()
{
    FILE *conf=NULL;
    conf=fopen("client.conf","w");
    fprintf(conf,"ip:%s$hostname:%s$NULL\n",server_ip,server_hostname);
    fclose(conf);
    return 0;
}

/**************************************************************************************
 *lit le fichier de conf et ecrit les variables gloables server_ip et server_hostname  *
 *                                                                                     *
 *                                                                                     *
 **************************************************************************************/

int lecture_fichier_conf()
{
    char lecture[1024];
    char decoupage_1[20][100];
    char decoupage_2[20][100];
    FILE *conf=NULL;
    conf=fopen("client.conf","r");
    if(conf==NULL)
    {
	/*printf("Le fichier de configuration n'existe pas.\n");
	  printf("Un nouveau fichier de configuration vierge ");
	  printf("va être créer.\n");*/
	conf=fopen("client.conf","w");
	fprintf(conf,"ip:NULL$hostname:NULL$NULL\n");
	sprintf(server_ip,"NULL");
	sprintf(server_hostname,"NULL");
	fclose(conf);
    }
    else
    {
	fgets(lecture,1024,conf);
	clearretourchariot(lecture,strlen(lecture));
	decoupageCommande(lecture,"$",decoupage_1);
	//Recuperation ip
	decoupageCommande(decoupage_1[0],":",decoupage_2);
	sprintf(server_ip,"%s",decoupage_2[1]);
	//Recuperation hostname
	decoupageCommande(decoupage_1[1],":",decoupage_2);
	sprintf(server_hostname,"%s",decoupage_2[1]);
	fclose(conf);
    }
    printf("server_ip = %s $ server_hostname = %s\n",server_ip,server_hostname);
    return 0;
}

/**************************************************************************************
 *Interception et gestion du signal SIGINT pour arreter proprement le serveur          *
 *                                                                                     *
 *                                                                                     *
 **************************************************************************************/

sigset_t set_masque_SIGINT ( )
{
    sigset_t sigmask, empty_mask;
    struct sigaction sa;
    sigemptyset(&sigmask);
    sigaddset(&sigmask,SIGINT);
    if(sigprocmask(SIG_BLOCK,&sigmask,NULL) == -1)
    {
	syslog(LOG_ERR,"%s",strerror(errno));
	arret_client(-1);
    }
    sa.sa_flags=0;
    sa.sa_handler=child_sig_handler;
    sigemptyset(&sa.sa_mask);
    if(sigaction(SIGINT,&sa,NULL)==-1)
    {
	syslog(LOG_ERR,"%s",strerror(errno));
	arret_client(-1);
    }
    sigemptyset(&empty_mask);
    return empty_mask;
}

/**************************************************************************************
 *suite voir set_masque_SIGINT                                                         *
 *                                                                                     *
 *                                                                                     *
 **************************************************************************************/

void child_sig_handler ( int signum )
{
    if(signum==SIGINT)
    {
	arret_client(0);
    }
}

/**************************************************************************************
 *                                                                                     *
 *                                                                                     *
 *                                                                                     *
 **************************************************************************************/

void traitement_quit ( )
{
    arret_client(0);
}

/**************************************************************************************
 *Aiguillage des commandes écrites au clavier                                          *
 *                                                                                     *
 *                                                                                     *
 **************************************************************************************/

int traitement_entree_clavier ( )
{
    int nbParam=0;
    char buffer[1024];
    char splitParam[20][100];
    lectureSTDIN(buffer);
    nbParam=decoupageCommande(buffer," ",splitParam);
    if(nbParam>0)
    {
	if(!strcmp(splitParam[0],"add"))
	{
	    traitement_add(splitParam,nbParam);
	}
	else if(!strcmp(splitParam[0],"register")
		|| !strcmp(splitParam[0],"rg"))
	{
	    if(traitement_register() == -1)
	    {
		printf("Une erreur est survenue veuillez consulter les logs\n");
	    }
	    else
	    {
		printf("Register effectué\n");
	    }
	}
	else if(!strcmp(splitParam[0],"ls")
		|| !strcmp(splitParam[0],"show"))
	{
	    traitement_show();
	}
	else if(!strcmp(splitParam[0],"quit")
		|| !strcmp(splitParam[0],"q"))
	{
	    traitement_quit();
	}
	else
	{
	    printf("L'utilisateur a écrit [%s]\n",buffer);
	}
    }
    return 0;
}

/**************************************************************************************
 *affichage des infos du server                                                        *
 *                                                                                     *
 *                                                                                     *
 **************************************************************************************/

int traitement_show()
{
    printf("server_ip = %s $ server_hostname = %s\n",server_ip,server_hostname);
    return 0;
}

/**************************************************************************************
 *ajout de l'adresse IP du server pour l'enregistrement                                *
 *                                                                                     *
 *                                                                                     *
 **************************************************************************************/

int traitement_add(char splitParam[20][100] , int nbParam )
{
    if(nbParam != 2)
    {
	printf("usage : add <adresse_ip_du_server>\n");
	return -1;
    }
    if(!isBufferAnIp(splitParam[1]))
    {
	printf("L'adresse Ip n'est pas valide\n");
	return -1;
    }
    sprintf(server_ip,"%s",splitParam[1]);
    sprintf(server_hostname,"%s","NULL");
    return 0;
}

/**************************************************************************************
 *auiguillage des connections et requetes sur le port 6000                             *
 *                                                                                     *
 *
 **************************************************************************************/

int connection_client_1 ( )
{
    int i;
    Communication com_in;
    clean_Communication(&com_in);
    if(!init_accept(&sock_out,sock_in_1))
    {
	printf("Connection entrante établie avec le serveur ");
	printf("(adresse ip : %s) ",inet_ntoa(sock_out.sin.sin_addr));
	printf("sur le port : %d\n",PORT_CLIENT_1);
	usleep(100);
	lecturesock(sock_out,(void*)&com_in,sizeof(com_in));
	if(!strcmp(com_in.header,"Request_Add"))
	{
	    request_add(com_in);
	}
	else
	{
	    printf("Le serveur a écrit: %s\n",com_in.header);
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
	return 0;
    }
    else
    {
	printf("ERREUR : une erreur est survenue ");
	printf("lors de la connection du serveur ");
	printf("sur la socket d'écoute 1\n");
	close(sock_out.sock);
	return 1;
    }
}

/**************************************************************************************
 *auiguillage des connections et requetes sur le port 6000                             *
 *                                                                                     *
 *                                                                                     *
 **************************************************************************************/

int connection_client_2 ( )
{
    ssize_t nb_bytes;
    int i;
    Communication com_in,com_out;
    clean_Communication(&com_in);
    clean_Communication(&com_out);
    if(!init_accept(&sock_out,sock_in_2))
    {
	usleep(100);
	nb_bytes=read(sock_out.sock,(void*)&com_in,sizeof(com_in));
	if(nb_bytes==-1)
	{
	    syslog(LOG_ERR,"connection_client_2() : %s",strerror(errno));
	    close(sock_out.sock);
	    return -1;
	}
	else
	{
	    if(!strcmp(com_in.header,"execute"))
	    {
		if(com_in.argc==3 
			&& (strcmp(com_in.arg[0],"") && strcmp(com_in.arg[0],"NULL"))
			&& (strcmp(com_in.arg[1],"") && strcmp(com_in.arg[1],"NULL"))
			&& (strcmp(com_in.arg[2],"") && strcmp(com_in.arg[2],"NULL")) 
		  )
		{
		    if(request_exe(com_in)==-1)
		    {
			close(sock_out.sock);
			return -1;
		    }
		    close(sock_out.sock);
		    return 0;
		}
		else
		{
		    syslog(LOG_ERR,"connection_client_2() : requete execute non conforme");
		    close(sock_out.sock);
		    return -1;
		}
	    }
	    else
	    {
		printf("Le serveur a écrit: %s\n",com_in.header);
		if(com_in.argc>0)
		{
		    printf("Ainsi que %d arguments\n",com_in.argc);
		    for(i=0;i<com_in.argc;i++)
		    {
			printf("arg[%d] = %s\n",i,com_in.arg[i]);
		    }
		}
		close(sock_out.sock);
		return 0;
	    }
	}
    }
    else
    {
	syslog(LOG_ERR,"connection_client_2() : %s",strerror(errno));
	close(sock_out.sock);
	return -1;
    }
}

/**************************************************************************************
 *traitement d'une connection sur le port 6000 lorsque le server procede               *
 *a l'enregistrement                                                                   *
 *                                                                                     *
 **************************************************************************************/

void request_add ( Communication com_in )
{
    Communication com_out;
    clean_Communication(&com_out);
    printf("Le serveur veut ajouter ce client à sa liste\n");
    sprintf(server_ip,"%s",inet_ntoa(sock_out.sin.sin_addr));
    printf("server_ip = %s $ ",inet_ntoa(sock_out.sin.sin_addr));
    sprintf(server_hostname,"%s",com_in.arg[0]);
    printf("server_hostname = %s\n",com_in.arg[0]);
    sprintf(com_out.header,"Request_Add_2");
    com_out.argc=2;
    sprintf(com_out.arg[0],"%s",HOSTNAME);
    sprintf(com_out.arg[1],"%s",VERSION);
    write(sock_out.sock,(void*)&com_out,sizeof(com_out));
    usleep(100);
    clean_Communication(&com_in);
    lecturesock(sock_out,(void*)&com_in,sizeof(com_in));
    if(!strcmp(com_in.header,"Request_Add_3"))
    {
	if(!strcmp(com_in.arg[0],"ok"))
	{
	    printf("Inscripion réussite\n");
	}
	else
	{
	    printf("Une erreur est survenue ");
	    printf("pendant l'inscription\n");
	}
    }
    else
    {
	printf("Une erreur est survenue pendant le protocole d'inscription\n");
    }
}

/**************************************************************************************
 *reception du code d'un script de la prt du server                                    *
 *                                                                                     *
 *                                                                                     *
 **************************************************************************************/

int reception_script(char *path)
{
    int i;
    FILE *script_client=NULL;
    char buffer[10];
    memset(buffer,0,10);
    sprintf(buffer,"ok");
    EnvoiScript data_in;
    script_client=fopen(path,"wb");
    if(script_client==NULL)
    {
	syslog(LOG_ERR,"%s",strerror(errno));
	return -1;
    }
    do
    {
	if(read(sock_out.sock,(void*)&data_in,sizeof(data_in)) == -1)
	{
	    syslog(LOG_ERR,"erreur lors de la lecture des paquetes envoyés par le server");   
	    return -1;
	}
	for(i=0;i<data_in.size;i++)
	{
	    fputc(data_in.contents[i],script_client);
	}
	if(write(sock_out.sock,buffer,strlen(buffer)) == -1)
	{
	    syslog(LOG_ERR,"erreur lors de l'envoi d'un acquitement ok au server");   
	    return -1;
	}
    }while(!data_in.end);
    fclose(script_client);
    printf("Script recu\n");
    chmod(path,S_IRUSR | S_IWUSR | S_IXUSR );
    printf("Droit script modifie\n");
    return 0;
}

/**************************************************************************************
 *execution d'un script lorsque le server le demande sur le port 6001                  *
 *                                                                                     *
 *                                                                                     *
 **************************************************************************************/

int request_exe ( Communication com_in )
{
    int size=0;
    char lecture;
    Communication com_out;
    clean_Communication(&com_out);
    EnvoiScript data_out;
    FILE *pipe;
    FILE *file;
    char path[100];
    char name[100],hash[100],secret[100];
    char hash2[40];
    char ok[10];
    sprintf(name,"%s",com_in.arg[0]);
    sprintf(hash,"%s",com_in.arg[1]);
    sprintf(secret,"%s",com_in.arg[2]);
    /*printf("Le server souhaite l'execution d'un script\n");
      printf("name = [%s]\n",name);
      printf("hash = [%s]\n",hash);
      printf("secret = [%s]\n",secret);*/
    sprintf(path,"script_client/%s",com_in.arg[0]);
    file=fopen(path,"r");
    if(file!=NULL)
    {
	fclose(file);
	printf("Le script est disponible\n");
	MD5_Hash(hash2,sizeof(hash2),path);
	if(strcmp(hash,hash2))
	{
	    printf("Les hashs ne correspondent pas!\n");
	    printf("Demande du script au serveur\n");
	    sprintf(com_out.header,"execute");
	    com_out.argc=1;
	    sprintf(com_out.arg[0],"nofile");
	    write(sock_out.sock,(void*)&com_out,sizeof(com_out));
	    reception_script(path);
	}
	else
	{
	    sprintf(com_out.header,"execute");
	    com_out.argc=1;
	    sprintf(com_out.arg[0],"ok");
	    write(sock_out.sock,(void*)&com_out,sizeof(com_out));
	}
    }
    else
    {
	printf("Demande du script au serveur\n");
	sprintf(com_out.header,"execute");
	com_out.argc=1;
	sprintf(com_out.arg[0],"nofile");
	write(sock_out.sock,(void*)&com_out,sizeof(com_out));
	reception_script(path);
    }
    close(sock_out.sock);
    /*
     * Ajouter un traitement
     */
    usleep(100);
    // FIN VERIFICATION PRESENCE SCRIPT (ET RECEPTION SI BESOIN)
    if(init_connect(&sock_out,server_ip,PORT_SERV_2))
    {
	printf("ERREUR : Un erreur est survenue lors ");
	printf("de la connection avec le serveur\n");
    }
    else
    {
	data_out.end=0;
	memset(data_out.header,0,1024);
	memset(data_out.contents,0,1024);
	printf("La connection avec le serveur a reussi\n");
	sprintf(com_out.header,"envoi_script");
	com_out.argc=2;
	sprintf(com_out.arg[0],"ask");
	sprintf(com_out.arg[1],"%s",secret);
	write(sock_out.sock,(void*)&com_out,sizeof(com_out));
	usleep(100);
	clean_Communication(&com_in);
	lecturesock(sock_out,(void*)&com_in,sizeof(com_in));
	if(!strcmp(com_in.header,"envoi_script")
		&& !strcmp(com_in.arg[0],"allow")
		&& com_in.argc==1)
	{
	    if((pipe=popen(path,"r"))==NULL)
	    {
		syslog(LOG_ERR,"request_exe() : %s",strerror(errno));
		close(sock_out.sock);
		return -1;
	    }
	    else
	    {
		sprintf(data_out.header,"envoi_script");
		size=0;
		data_out.end=0;
		lecture=fgetc(pipe);
		if(lecture==EOF)
		{
		    data_out.size=0;
		    memset(data_out.contents,0,1024);
		    write(sock_out.sock,(void*)&data_out,sizeof(data_out));
		    if(read(sock_out.sock,ok,9)==-1)
		    {
			syslog(LOG_ERR,"%s\n",strerror(errno));
			pclose(pipe);
			close(sock_out.sock);
			return -1;
		    }
		    if(strcmp(ok,"ok"))
		    {
			syslog(LOG_ERR,"probleme acquietement par le serveur");
			pclose(pipe);
			close(sock_out.sock);
			return -1;
		    }
		}
		else
		{
		    while(lecture!=EOF)
		    {
			data_out.contents[size]=lecture;
			size++;
			lecture=fgetc(pipe);
			if(lecture==EOF)
			{
			    data_out.end=1;
			}
			if(size==1024 || data_out.end==1)
			{
			    data_out.size=size;
			    write(sock_out.sock,(void*)&data_out,sizeof(data_out));
			    if(read(sock_out.sock,ok,9)==-1)
			    {
				syslog(LOG_ERR,"%s\n",strerror(errno));
				pclose(pipe);
				close(sock_out.sock);
				return -1;
			    }
			    if(strcmp(ok,"ok"))
			    {
				syslog(LOG_ERR,"probleme acquietement par le serveur");
				pclose(pipe);
				close(sock_out.sock);
				return -1;
			    }
			    memset(data_out.contents,0,1024);
			    data_out.size=0;
			    size=0;
			}
		    }
		    printf("Le retour du script a été envoyé au serveur\n");
		}
	    }
	    pclose(pipe);
	    lecturesock(sock_out,(void*)&com_in,sizeof(com_in));
	    if(!strcmp(com_in.header,"envoi_script")
		    && !strcmp(com_in.arg[0],"end")
		    && com_in.argc==1)
	    {
		printf("Le serveur a bien recu le resultat du script\n");
	    }
	    else
	    {
		printf("Le serveur n'a pas bien recu le resultat du script\n");
	    }
	}
    }
    close(sock_out.sock);
    return 0;
}

/**************************************************************************************
 *                                                                                     *
 *                                                                                     *
 *                                                                                     *
 **************************************************************************************/

// --- FONCTIONS ---------------------------------------------------------------- //
