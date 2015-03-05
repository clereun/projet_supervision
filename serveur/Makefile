# Makefile projet securite 4A
# Gestion à distance d'un "parc" informatique
# Le Reun Charles - Herve Coulon
# Partie Serveur

CC     = gcc -Wall -Wextra
THREAD = -lpthread
MYSQL  = `mysql_config --cflags --libs`
SSL    = -lssl -lcrypto

.PHONY: delete, install, clean, mrproper, rebuild

all : bin/server

install : archi all

archi :
	mkdir ./bin/
	mkdir ./obj/
	mkdir ./bin/retour_script
	ln -s ../script bin/script
	ln -s ../server.conf bin/server.conf

bin/server : obj/server.o obj/ressource.o
	$(CC) -o bin/server obj/server.o obj/ressource.o $(THREAD) $(MYSQL) $(SSL)

obj/server.o : src/server.c src/server.h src/ressource.h
	$(CC) -o obj/server.o -c src/server.c $(MYSQL)

obj/ressource.o : src/ressource.c src/ressource.h
	$(CC) -o obj/ressource.o -c src/ressource.c

rebuild : mrproper all

clean :
	rm -rf obj/*

mrproper : clean
	rm -rf bin/server

delete :
	rm -rf obj
	rm -rf bin
