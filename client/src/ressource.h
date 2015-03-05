/*
 * DESCRIPTION DU FICHIER
 * Name : ressource.h
 */

#ifndef __RESSOURCE_H__
#define __RESSOURCE_H__

// ------ INCLUDE ------------------------------------------------------------------ //
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>
#include <openssl/ssl.h>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/select.h>
#include <signal.h>
#include <fcntl.h>
#include <syslog.h>
// ------ INCLUDE ------------------------------------------------------------------ //

// ------ DEFINE ------------------------------------------------------------------- //
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define BUF_SIZE 1024
#define PORT_CLIENT_1 6000
#define PORT_CLIENT_2 6001
#define PORT_SERV_1 7000
#define PORT_SERV_2 7001
#define PORT_WEB 7002
#define C_PORT_CLIENT_1 "6000"
#define C_PORT_CLIENT_2 "6001"
#define C_PORT_SERV_1 "7000"
#define C_PORT_SERV_2 "7001"
#define C_PORT_WEB "7002"
#define SIZE_CONTENTS 1024
#define BUG(a) sprintf(a,"Dans le fichier %s dans la fonction %s à la ligne %d\n",__FILE__, __FUNCTION__, __LINE__);
#define BUGEUH printf("Dans le fichier %s dans la fonction %s à la ligne %d\n",__FILE__, __FUNCTION__, __LINE__);
// ------ DEFINE ------------------------------------------------------------------- //

// ------ DEFINITIONS TYPEDEF ------------------------------------------------------ //
typedef int SOCKET;
typedef struct sockaddr_un SOCKADDR_UN;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
typedef struct in_addr IN_ADDR;
// ------ DEFINITIONS TYPEDEF ------------------------------------------------------ //

// ------ DEFINITIONS STRUCTURE ---------------------------------------------------- //
typedef struct Socket {
    SOCKET sock;
    SOCKADDR_IN sin;
} Socket;
typedef struct Socket_un {
    SOCKET sock;
    SOCKADDR_UN sun;
} Socket_un;
typedef struct Communication {
    char header[100];
    int argc;
    char arg[10][100];
} Communication;
typedef struct EnvoiScript {
    char header[100];
    char end;
    int size;
    char contents[SIZE_CONTENTS];
} EnvoiScript;
// ------ DEFINITIONS STRUCTURE ---------------------------------------------------- //

// ------ PROTOTYPES DE FONCTIONS -------------------------------------------------- //
void Initialisation_SSL();
// ----------------------------------------
void Suppression_SSL();
// ----------------------------------------
void Fermeture_SSL(SSL *ssl);
// ----------------------------------------
void clean_Communication ( Communication *com );
// ----------------------------------------
int isBufferAnIp ( char *buffer );
// ----------------------------------------
int decoupageCommande ( const char *buffer, const char *separateur,char tab[20][100] );
// ----------------------------------------
int lecturesock ( Socket sock, char *buffer, int taill );
// ----------------------------------------
int clearretourchariot ( char *buffer, int taille );
// ----------------------------------------
int init_listean ( Socket *sock, int port);
// ----------------------------------------
int init_listean_unix ( Socket_un *sock,char *path);
// ----------------------------------------
int init_accept ( Socket *sock, Socket socket );
// ----------------------------------------
int init_accept_unix ( Socket_un *sock, Socket_un socket );
// ----------------------------------------
int init_connect ( Socket *sock, char *ip, int port );
// ----------------------------------------
int init_connect_unix ( Socket_un *sock, char *path );
// ----------------------------------------
void lectureSTDIN ( char *buffer );
// ------ PROTOTYPES DE FONCTIONS -------------------------------------------------- //

#endif /* __RESSOURCE_H__ */
