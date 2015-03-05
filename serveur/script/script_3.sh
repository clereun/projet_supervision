#!/bin/bash

#recupère la table route sous la forme Destination$Passerelle$Genmask$Iface$
route | awk '{ print $1,$2,$3,$8"$" }' |  sed -e "/^Table/ d" |  sed -e "/^Destination/ d" | sed -e "s/\ /$/g"
