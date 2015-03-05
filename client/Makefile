# makefile projet securite 4A
# Gestion à distance d'un "parc" informatique
# Le Reun Charles - Herve Coulon

THREAD  = -lpthread
CC      = gcc -ldl -Wall -Wextra
SSL     = -lssl -lcrypto

.PHONY: delete, install, rebuild, clean, mrproper

all : bin/client

install : archi all

archi :
	mkdir ./bin/
	mkdir ./obj/
	mkdir ./bin/script_client/

bin/client : obj/client.o obj/ressource.o
	$(CC) -o bin/client obj/client.o obj/ressource.o $(THREAD) $(SSL)

obj/client.o : src/client.c src/client.h src/ressource.h
	$(CC) -o obj/client.o -c src/client.c

obj/ressource.o : src/ressource.c src/ressource.h
	$(CC) -o obj/ressource.o -c src/ressource.c

rebuild : mrproper all 

clean : 
	rm -rf obj/*

mrproper : clean
	rm -rf bin/client

delete :
	rm -rf obj
	rm -rf bin

