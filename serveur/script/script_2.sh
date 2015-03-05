#!/bin/bash

#utilisateur$date_de_co$date_deco$temps_de_co
last -dFw | sed -e "/^(unknown)/ d" | sed -e "/^reboot/ d" | sed -e "/^wtmp/ d" | sed -e "/^$/ d" | awk '{ print $1 "$" $6 " " $5 " " $8 " à " $7 "$" $12 " " $11 " " $14 " à " $13 $15}' | sed -e "s/(/$/g" | sed -e "s/)/$/g" | sed "/\ $/d" | sed -e "/[+]/d"