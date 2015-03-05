#!/bin/bash

apt-get update > /tmp/null
#Liste des paquets à mettre à jour, sous la forme PROGRAM$INSTALLED$AVAILABLE
apt-get --just-print upgrade 2>&1 | perl -ne 'if (/Inst\s([\w,\-,\d,\.,~,:,\+]+)\s\[([\w,\-,\d,\.,~,:,\+]+)\]\s\(([\w,\-,\d,\.,~,:,\+]+)\)? /i) {print "$1 $2 $3 \n"}' | sed -e "s/\ /$/g"


