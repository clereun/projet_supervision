<?php
try
{
  $options = array( PDO::MYSQL_ATTR_INIT_COMMAND => 'SET NAMES utf8',);
  $bdd = new PDO('mysql:host=localhost;dbname=PROJET_SECU','root','azerty', $options);
}
catch(Exception $e)
{
  die('Erreur : '.$e->getMessage());
}
  $requete_info_script_generale=$bdd->query("SELECT script_id FROM SCRIPT WHERE `SCRIPT`.`script_name`='script_0.sh';");
  $info_script_generale=$requete_info_script_generale->fetch();
  print_r($info_script_generale);
  echo $info_script_generale['script_id'];
?>