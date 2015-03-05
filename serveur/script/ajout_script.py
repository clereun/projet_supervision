#!/usr/bin/python
# -*- coding: utf-8 -*-

import MySQLdb as mdb
import sys
import hashlib

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


name=sys.argv[1]
hasher=hashlib.md5()
with open(sys.argv[1],'rb') as afile:
    buf=afile.read()
    hasher.update(buf)
descr=sys.argv[2]

con = mdb.connect('localhost', admin_bdd, mdp_bdd, 'PROJET_SECU')
cur = con.cursor()
cur.execute("INSERT INTO SCRIPT(script_name,script_hash,script_descr) VALUES('" + name +"','" + hasher.hexdigest() + "','" + descr + "')")
con.commit()
con.close()

