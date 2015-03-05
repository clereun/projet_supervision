#!/usr/bin/python
# -*- coding: utf-8 -*-

import MySQLdb as mdb
import sys

mdp_bdd=''
admin_bdd=''

source=open("script/secret","r")
for ligne in source:
    donnees = ligne.rstrip('\n\r').split("=")
    if donnees[0]=='admin_bdd':
        admin_bdd=donnees[1]
    elif donnees[0]=='mdp_bdd':
        mdp_bdd=donnees[1]
source.close()

source=open(sys.argv[1],"r")
id_client = sys.argv[2]

con = mdb.connect('localhost', admin_bdd, mdp_bdd, 'PROJET_SECU')
cur = con.cursor()
for ligne in source:
    donnees = ligne.rstrip('\n\r').split("$")
    cur.execute("INSERT INTO `PROJET_SECU`.`PAQUETS` (paq_nom_programme,cli_id) VALUES('" + donnees[0] + "','" + id_client + "')")



con.commit()
con.close()

source.close()