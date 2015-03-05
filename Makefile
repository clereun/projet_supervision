# makefile projet securite 4A
# Gestion à distance d'un "parc" informatique
# Le Reun Charles - Herve Coulon

MYSQL  = `mysql_config --cflags --libs`
CC     = gcc -ldl -Wall -Wextra
CIBLES = bin/server bin/client 

.PHONY: delete, install, rebuild, clean, mrproper

all : $(CIBLES)

install : archi
	@(cd serveur && $(MAKE) install)
	ln -s ../serveur/bin/server bin/server
	@(cd client && $(MAKE) install)
	ln -s ../client/bin/client bin/client

archi :
	mkdir ./bin

bin/server : ./bin
	@(cd serveur && $(MAKE) all)
	@rm -f bin/server
	@ln -s ../serveur/bin/server bin/server

bin/client : ./bin
	@(cd client && $(MAKE) all)
	@rm -f bin/client
	@ln -s ../client/bin/client bin/client

rebuild : mrproper all

clean :
	@(cd serveur && $(MAKE) clean)
	@(cd client && $(MAKE) clean)

mrproper :
	@(cd serveur && $(MAKE) mrproper)
	@(cd client && $(MAKE) mrproper)

delete :
	@(cd serveur && $(MAKE) delete)
	@(cd client && $(MAKE) delete)
	rm -rf bin
