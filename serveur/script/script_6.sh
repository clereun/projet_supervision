#!/bin/bash

#liste des services en écoute, protocol$adresse_locale$port$adresse_distante$port_distant$PID$nom_du_programme
netstat -nptav4 | grep LISTEN | awk '{ print $1,$4,$5,$7"$" }' | sed -e "s/\//$/g"  | sed -e "s/\ /$/g" | sed -e "s/:/$/g" | cut -d: -f 1,2,3,5