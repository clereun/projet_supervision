/*
 * DESCRIPTION DU FICHIER
 * Name : client.h 
 */

#ifndef __CLIENT_H__
#define __CLIENT_H__

// --- INCLUDE ------------------------------------------------------------------ //
#include "ressource.h"
#include <pthread.h>
#include <openssl/md5.h>
 
// --- INCLUDE ------------------------------------------------------------------ //

// --- DEFINE ------------------------------------------------------------------- //
#define HOSTNAME "slave"
#define VERSION "1.0.0.0"
// --- DEFINE ------------------------------------------------------------------- //

// --- DEFINITIONS STRUCTURES --------------------------------------------------- //
// --- DEFINITIONS STRUCTURES --------------------------------------------------- //

// --- VARIABLES GLOBALES ------------------------------------------------------- //
    // --Socket-listean-------------------------
	Socket sock_in_1;
	Socket sock_in_2;
	Socket sock_out;
    // --Socket-accept-et-connect---------------
	static volatile sig_atomic_t continu =1;
    // --ip-et-hostname-server------------------
	char server_ip[30];
	char server_hostname[30];
    // -----------------------------------------
// --- VARIABLES GLOBALES ------------------------------------------------------- //

// --- PROTOTYPES DE FONCTIONS -------------------------------------------------- //
void* thread_kill(void *arg);
// --------------------------------------
int lancement_thread_kill(pid_t pid);
// --------------------------------------
int initialisation_client();
//-----------------------------------------
int arret_client(int retour);
//-----------------------------------------
int reception_script(char *path);
//-----------------------------------------
int traitement_show();
//-----------------------------------------
int maj_conf();
//-----------------------------------------
int lecture_fichier_conf ( );
//-----------------------------------------
void request_add ( Communication com_in );
//-----------------------------------------
int request_exe ( Communication com_in );
//-----------------------------------------
sigset_t set_masque_SIGINT ( );
//-----------------------------------------
void child_sig_handler ( int signum );
//-----------------------------------------
int traitement_add (char splitParam[20][100] , int nbParam );
//-----------------------------------------
int traitement_register ( );
//-----------------------------------------
void traitement_quit ( );
//-----------------------------------------
int traitement_entree_clavier ( );
//-----------------------------------------
int connection_client_1 ( );
//-----------------------------------------
int connection_client_2 ( );
// --- PROTOTYPES DE FONCTIONS -------------------------------------------------- //

#endif /* __CLIENT_H__ */
