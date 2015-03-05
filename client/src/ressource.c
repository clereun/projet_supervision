/*
 * DESCRIPTION DU FICHIER
 * Name : ressource.c
 */

// --- INCLUDE ------------------------------------------------------------------ //
#include "ressource.h"
#include <openssl/ssl.h>
#include <openssl/bio.h>
#include <openssl/err.h>
// --- INCLUDE ------------------------------------------------------------------ //

// --- FONCTIONS ---------------------------------------------------------------- //



/**************************************************************************************
 *                                                                                     *
 *                                                                                     *
 *                                                                                     *
 **************************************************************************************/

 void Initialisation_SSL() {
    SSL_load_error_strings();
    SSL_library_init();
    OpenSSL_add_all_algorithms();
}

 /**************************************************************************************
 *                                                                                     *
 *                                                                                     *
 *                                                                                     *
 **************************************************************************************/

 void Suppression_SSL() {
    ERR_free_strings();
    EVP_cleanup();
}

/**************************************************************************************
 *                                                                                     *
 *                                                                                     *
 *                                                                                     *
 **************************************************************************************/

 void Fermeture_SSL(SSL *ssl) {
    SSL_shutdown(ssl);
    SSL_free(ssl);
}

/**************************************************************************************
 *                                                                                     *
 *                                                                                     *
 *                                                                                     *
 **************************************************************************************/

void clean_Communication ( Communication *com )
{
    int i;
    memset(com->header,0,1024);
    com->argc=0;
    for(i=0;i<10;i++)
    {
	memset(com->arg[i],0,100);
    }
}

/**************************************************************************************
 *                                                                                     *
 *                                                                                     *
 *                                                                                     *
 **************************************************************************************/

int isBufferAnIp ( char *buffer )
{
    int i,j;
    int taille;
    int stringToInt;
    int nbDecoupage;
    char liste[1024];
    char tmps[20][100];
    memcpy(liste,buffer,1024);
    nbDecoupage=decoupageCommande(liste,".",tmps);
    if(nbDecoupage!=4)
    {
	return 0;
    }
    for(i=0;i<nbDecoupage;i++)
    {
	stringToInt=0;
	taille=strlen(tmps[i]);
	for(j=0;j<taille;j++)
	{
	    stringToInt*=10;
	    stringToInt+=tmps[i][j]-'0';
	}
	if(stringToInt<0 || stringToInt>255)
	{
	    return 0;
	}
    }
    return 1;
}

/**************************************************************************************
 *                                                                                     *
 *                                                                                     *
 *                                                                                     *
 **************************************************************************************/

int decoupageCommande(const char *buffer,const char *separateur,char tab[20][100])
{
    int nb=0;
    char temp[1024];
    char *temp2=NULL;
    if(buffer!=NULL && separateur !=NULL)
    {
	sprintf(temp,"%s",buffer);
	if((temp2=strtok(temp,separateur))!=NULL)
	{
	    sprintf(tab[nb],"%s",temp2);
	    nb++;
	    while( (temp2=strtok(NULL,separateur)) !=NULL)
	    {
		sprintf(tab[nb],"%s",temp2);
		nb++;
	    }
	}
	else
	{
	    return -1;
	}
    }
    return nb;
}

/**************************************************************************************
 *                                                                                     *
 *                                                                                     *
 *                                                                                     *
 **************************************************************************************/

int lecturesock(Socket sock,char *buffer,int taille)
{
    int n;
    if( ( n=read(sock.sock,buffer,taille) ) <= -1)
    {
	perror("read()");
	return -1;
	//exit(errno);
    }
    buffer[n]=0;
    return n;
}

/**************************************************************************************
 *                                                                                     *
 *                                                                                     *
 *                                                                                     *
 **************************************************************************************/

int clearretourchariot(char *buffer,int taille)
{
    int i=0;
    int a=1;
    while(a && i<taille)
    {
	if(buffer[i]=='\n')
	{
	    buffer[i]=0;
	    a=0;
	}
	i++;
    }
    if(i==taille)
    {
	return -1;
    }
    else
    {
	return i-1;
    }
}

/**************************************************************************************
 *                                                                                     *
 *                                                                                     *
 *                                                                                     *
 **************************************************************************************/

int init_listean(Socket *sock,int port)
{
    int opt=1;
    sock->sock = socket ( AF_INET , SOCK_STREAM , 0 );
    if ( sock->sock == -1 )
    {
	return 1;
    }
    setsockopt(sock->sock, SOL_SOCKET, SO_REUSEADDR,&opt,sizeof(int));    
    sock->sin.sin_addr.s_addr=htonl(INADDR_ANY);
    sock->sin.sin_family=AF_INET;
    sock->sin.sin_port=htons(port);
    if( bind( sock->sock, (SOCKADDR*)&sock->sin, sizeof sock->sin ) == -1 )
    {
	return 2;
    }
    if( listen(sock->sock, 5 ) == -1 )
    {
	return 3;
    }
    return 0;
}

/**************************************************************************************
 *                                                                                     *
 *                                                                                     *
 *                                                                                     *
 **************************************************************************************/

int init_listean_unix(Socket_un *sock,char *path)
{
    int opt=1;
    socklen_t len;
    sock->sock = socket ( AF_UNIX , SOCK_STREAM , 0 );
    if ( sock->sock == -1 )
    {
	perror("socket()");
	return 1;
    }
    setsockopt(sock->sock, SOL_SOCKET, SO_REUSEADDR,&opt,sizeof(int));    
    sock->sun.sun_family=AF_UNIX;
    sprintf(sock->sun.sun_path,"%s",path);
    unlink(sock->sun.sun_path);
    len=strlen(sock->sun.sun_path)+sizeof(sock->sun.sun_family);
    if( bind( sock->sock, (SOCKADDR*)&sock->sun,len) == -1 )
    {
	perror("bind()");
	return 2;
    }
    if( listen(sock->sock, 5 ) == -1 )
    {
	perror("listen()");
	return 3;
    }
    return 0;
}
/**************************************************************************************
 *                                                                                     *
 *                                                                                     *
 *                                                                                     *
 **************************************************************************************/

int init_accept(Socket *sock,Socket socket)
{
    socklen_t sinsize=sizeof sock->sin;
    sock->sock = accept(socket.sock,(SOCKADDR*)&sock->sin,&sinsize);
    if(sock->sock==-1)
    {
	// TODO : a tej apres avoir modif le client
	//syslog(LOG_ERR,"%s\n",strerror(errno));
	//return -1;
	return 1;
    }
    return 0;
}

/**************************************************************************************
 *                                                                                     *
 *                                                                                     *
 *                                                                                     *
 **************************************************************************************/

int init_accept_unix(Socket_un *sock,Socket_un socket)
{
    socklen_t sinsize=sizeof sock->sun;
    sock->sock = accept(socket.sock,(SOCKADDR*)&sock->sun,&sinsize);
    if(sock->sock==-1)
    {
	// TODO : a tej apres avoir modif le client
	//syslog(LOG_ERR,"%s\n",strerror(errno));
	//return -1;
	return 1;
    }
    return 0;
}

/**************************************************************************************
 *                                                                                     *
 *                                                                                     *
 *                                                                                     *
 **************************************************************************************/

int init_connect(Socket *sock,char *ip,int port)
{
    sock->sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock->sock == INVALID_SOCKET)
    {
	perror("socket()");
	return 1;
    }
    sock->sin.sin_addr.s_addr = inet_addr(ip);
    sock->sin.sin_port = htons(port);
    sock->sin.sin_family = AF_INET;
    if(connect(sock->sock,(SOCKADDR *) &sock->sin, sizeof(SOCKADDR)) == SOCKET_ERROR)
    {
	perror("connect()");
	return 2;
    }
    return 0;
}

/**************************************************************************************
 *                                                                                     *
 *                                                                                     *
 *                                                                                     *
 **************************************************************************************/

int init_connect_unix(Socket_un *sock,char *path)
{
    socklen_t len;
    sock->sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if(sock->sock == INVALID_SOCKET)
    {
	perror("socket()");
	return 1;
    }
    sock->sun.sun_family=AF_UNIX;
    sprintf(sock->sun.sun_path,"%s",path);
    len=strlen(sock->sun.sun_path) + sizeof(sock->sun.sun_family);
    if(connect(sock->sock,(SOCKADDR *) &sock->sun,len) == SOCKET_ERROR)
    {
	perror("connect()");
	return 2;
    }
    return 0;
}

/**************************************************************************************
 *                                                                                     *
 *                                                                                     *
 *                                                                                     *
 **************************************************************************************/

void lectureSTDIN(char *buffer)
{
    fgets(buffer, BUF_SIZE - 1, stdin);
    {
	char *p = NULL;
	p = strstr(buffer, "\n");
	if(p != NULL)
	{
	    p = 0;
	}
	else
	{
	    /* fclean */
	    buffer[BUF_SIZE - 1] = 0;
	}
    }
    int i=0;
    while(buffer[i]!='\n')
    {
	i++;
    }
    buffer[i]='\0';
}
// --- FONCTIONS ---------------------------------------------------------------- //
