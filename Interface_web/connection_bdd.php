<?php
try
{
  $options = array( PDO::MYSQL_ATTR_INIT_COMMAND => 'SET NAMES utf8',);
  $bdd = new PDO('mysql:host=localhost;dbname=PROJET_SECU',$user_bdd,$mdp_bdd, $options);
}
catch(Exception $e)
{
  die('Erreur : '.$e->getMessage());
}
?>