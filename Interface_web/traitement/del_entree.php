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
if(isset($_GET['id']))
{
  try
  {
    $bdd = new PDO('mysql:host=localhost;dbname=PROJET_SECU', $user_bdd,$mdp_bdd);
  }
  catch(Exception $e)
  {
    $_SESSION['msg_del']='Erreur : '.$e->getMessage();
  }
  $reponse=$bdd->query("DELETE FROM CLIENTS Where cli_id='".$_GET['id']."'");
}
header('Location: ../accueil.php');
?>
