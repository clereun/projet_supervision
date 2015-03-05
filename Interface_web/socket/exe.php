<?php

$host = "127.0.0.1";
$port = "7002";

$socket = socket_create(AF_INET, SOCK_STREAM, SOL_TCP)
    or die("Unable to create socket\n");

socket_connect ( $socket, $host, $port);
socket_write ( $socket , "exe ".$_GET['id']." ".$_GET['script']);

sleep(1);

header('Location: ../details.php?id='.$_GET['id']);

?>
