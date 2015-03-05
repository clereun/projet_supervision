/*
 * DESCRIPTION DU FICHIER
 * Name : server.h
 */

#ifndef __SERVER_H__
#define __SERVER_H__ 

// --- INCLUDE ------------------------------------------------------------------ //
#include "ressource.h"
#include <my_global.h>
#include <mysql.h>
#include <pthread.h>
// --- INCLUDE ------------------------------------------------------------------ //

// --- DEFINE ------------------------------------------------------------------- //
#define HOSTNAME_SERVER "MASTER"
#define VERSION_SERVER "1.0.0.0"
// --- DEFINE ------------------------------------------------------------------- //

// --- DEFINITIONS STRUCTURES --------------------------------------------------- //
typedef struct Parametres {
    int sNumeroLigne;
    char sIp[1024];
    FILE *sFichier;
}Parametres;
// --- DEFINITIONS STRUCTURES --------------------------------------------------- //

// --- VARIABLES GLOBALES ------------------------------------------------------- //
	char buffer_info[1024];
    // --Socket-AF_INET----------------------
	Socket sock_in_1;
	Socket sock_in_2;
	Socket sock_web;
	Socket sock_out;
    // --Socket_un-AF_UNIX-------------------
	Socket_un sock_in_unix;
	Socket_un sock_out_unix;
	Socket_un internal;
    // --Variable-pour-signaux---------------
	static volatile sig_atomic_t continu = 1;
    // --Variables-globales-bdd--------------
	MYSQL *con = NULL;
	char user_bdd[100];
	char mdp_bdd[100];
// --- VARIABLES GLOBALES ------------------------------------------------------- //

// --- PROTOTYPES DE FONCTIONS -------------------------------------------------- //
int traitement_entree_interne();
// --------------------------------------
int fermeture_socket_ecoute();
// --------------------------------------
int is_client_registered(char *id_client);
// --------------------------------------
int traitement_reponse_script(char *secret);
// --------------------------------------
void* thread_timer(/*void *arg*/);
// --------------------------------------
pthread_t lancement_thread_timer();
// --------------------------------------
void* thread_kill(void *arg);
// --------------------------------------
int lancement_thread_kill(pid_t pid);
// --------------------------------------
MYSQL* connection_bdd();
// --------------------------------------
int sauvegarde_secret(char *cli_id,char *secret,char *script_name);
// --------------------------------------
int verification_secret(char *cli_id,char *secret);
// --------------------------------------
void generation_secret(char *secret,int taille);
// --------------------------------------
int recuperation_info_script(char *id_script,char *name,char *hash);
// --------------------------------------
int recuperation_ip_client(char *id_client,char *ip_client);
// --------------------------------------
int envoi_script(char *file);
// --------------------------------------
int initialisation_serveur();
// --------------------------------------
int traitement_request_register(Communication com_in);
// --------------------------------------
int traitement_reception_script(Communication com_in);
// --------------------------------------
int traitement_entree_1();
// --------------------------------------
int traitement_entree_2();
// --------------------------------------
int traitement_entree_web();
// --------------------------------------
int traitement_entree_unix();
// --------------------------------------
int verifier_dossier_lancement ( char *dossier_programme );
// --------------------------------------
void dossier_lancement ( char *chemin_programme, char *dossier );
// --------------------------------------
sigset_t set_masque_SIGINT ( );
// --------------------------------------
void child_sig_handler ( int signum );
// --------------------------------------
int traitement_update ( );
// --------------------------------------
int enregistrement_client ( char *id_client);
// --------------------------------------
int traitement_add ( char splitParam[20][100], int nbParam );
// --------------------------------------
int add_client ( char *buffer );
// --------------------------------------
int traitement_del ( char splitParam[20][100], int nbParam );
// --------------------------------------
int del_client ( char *buffer );
// --------------------------------------
int traitement_show_script ( );
// --------------------------------------
int traitement_show ( );
// --------------------------------------
void arret_serveur ( int retour );
// --------------------------------------
int traitement_entree_clavier ( );
// --------------------------------------
int traitement_bonjour ( char splitParam[20][100], int nbParam );
// --------------------------------------
int traitement_exe ( char splitParam[20][100], int nbParam );
// --------------------------------------
int bonjour_client ( );
// --------------------------------------
int fork_exe (char* cli_id,char *name,char *hash,char *secret);
// --- PROTOTYPES DE FONCTIONS -------------------------------------------------- //

#endif /* __SERVER_H__ */
