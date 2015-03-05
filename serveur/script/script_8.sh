#!/bin/bash

########################## Sauvegarde ##############################

# sudo -s iptables-save -c

# Si déjà modifié je le réinitialise
iptables -F
iptables -X

########################## State-full ##############################

iptables -A INPUT -p tcp ! --syn -m state --state NEW -j DROP
iptables -A INPUT -m state --state ESTABLISHED,RELATED -j ACCEPT
iptables -A OUTPUT -m state --state ESTABLISHED,RELATED -j ACCEPT

# Pour permettre à une connexion déjà ouverte de recevoir du trafic :
iptables -A INPUT -m conntrack --ctstate ESTABLISHED -j ACCEPT

########################## Lo ##############################

iptables -A INPUT -i lo -j ACCEPT;	iptables -A OUTPUT -o lo -j ACCEPT

########################## SSH ##############################

iptables -A INPUT -p tcp --dport 22 -m state --state NEW -j ACCEPT

########################## WEB ##############################

iptables -A INPUT -p tcp --dport 80 -m state --state NEW -j ACCEPT
iptables -A INPUT -p tcp --dport 443 -m state --state NEW -j ACCEPT

########################## DNS ##############################

iptables -A OUTPUT -p udp --dport 53 -m state --state NEW -j ACCEPT
iptables -A OUTPUT -p tcp --dport 53 -m state --state NEW -j ACCEPT

########################## LOG ##############################

iptables -A INPUT -j LOG

########################## Ping ##############################

# Autorise les pings 
iptables -A INPUT -p icmp -j ACCEPT

########################## Lutte contre les attaques DoS  ##############################

# lutter contre le SYS-FLOOD
iptables -A INPUT -p tcp --syn -m limit --limit 1/m -j ACCEPT

# lutter contre le balayage furtif de port
iptables -A INPUT -p tcp --tcp-flags SYN,ACK,FIN,RST RST -m limit --limit 1/s -j ACCEPT

# lutter contre le ping de la mort
iptables -A INPUT -p icmp --icmp-type echo-request -m limit --limit 1/s -j ACCEPT

# lutter contre l'UDP flood
iptables -A INPUT -p udp -m limit --limit 1/s -j ACCEPT
########################## Bloque ##############################

# Pour bloquer tout ce qui n'est pas dans les règles précédentes
iptables -P INPUT DROP
iptables -P FORWARD DROP
iptables -P OUTPUT DROP

