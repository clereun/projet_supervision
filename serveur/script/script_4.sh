#!/bin/bash

#recupère la liste des paquets installés sur le client
dpkg --get-selections | awk '{ print $1"$"}'
