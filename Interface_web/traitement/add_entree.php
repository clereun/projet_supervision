<?php
session_start();
if(isset($_SESSION['state'])) 
{
  if($_SESSION['state'] == 'new')
  {
    $_SESSION=array();
    session_destroy();
    header('Location: ../index.php');
  }
}
else
{
  $_SESSION=array();
  session_destroy();
  header('Location: ../index.php');
}
include '../master.php';
if(isset($_POST['input_ip']))
{
  $_SESSION['msg_format']=0;
  $_SESSION['msg_taille_ip']=0;
  if($_POST['input_ip']=="")
  {
    $_SESSION['msg_taille_ip']=1;
  }
  else
  {
    $_SESSION['msg_taille_ip']=0;
    if(filter_var($_POST['input_ip'],FILTER_VALIDATE_IP))
    {
      $_SESSION['msg_format']=0;
      try
      {
	$bdd = new PDO('mysql:host=localhost;dbname=PROJET_SECU',
	  $user_bdd,$mdp_bdd);
      }
      catch(Exception $e)
      {
	$_SESSION['msg_del']='Erreur : '.$e->getMessage();
      }
      $reponse=$bdd->query("INSERT INTO CLIENTS (cli_ip) VALUES ('".
	$_POST['input_ip']."')");
    }
    else
    {
      $_SESSION['msg_format']=1;
    }
  }
}
header('Location: ../accueil.php');
?>
