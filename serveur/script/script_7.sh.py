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
li = []

con = mdb.connect('localhost', admin_bdd, mdp_bdd, 'PROJET_SECU')
cur = con.cursor()

for ligne in source:
	donnees = ligne.rstrip('\n\r').split("$")
	li.append(donnees[0])

cur.execute("INSERT INTO `PROJET_SECU`.`HASH_DOSSIERS` (hd_bin,hd_sbin,hd_usr_bin,hd_usr_sbin,cli_id) VALUES('" + li[0] + "','" + li[1] + "','" + li[2] + "','" + li[3] + "','" + id_client + "')")
cur.execute("INSERT INTO `PROJET_SECU`.`SECURITE` (secu_shadow,secu_passwd,cli_id) VALUES('" + li[4] + "','" + li[5] + "','" + id_client + "')")
 
con.commit()
con.close()

source.close()