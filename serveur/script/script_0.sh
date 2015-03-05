#!/bin/bash

#cli_hostname`,`cli_distrib, vers_logiciel,vers_distribution,vers_noyau
echo "$(hostname)$"$(lsb_release -i | cut -c17-)"$""1.0$"$(lsb_release -r | cut -c10-)"$"$(uname -r)"$"
