<?php
include 'master.php';
session_start();
if(!isset($_SESSION['state']))
{
  $_SESSION['state'] = 'new';
  $_SESSION['erreur_connection'] = 0;
}
if($_SESSION['state'] == 'connected')
{
  header('Location: accueil.php');
}
else if($_SESSION['state'] == 'new')
{
  if(isset($_POST['input_id']) AND isset($_POST['input_mdp']))
  {
    //if($_POST['input_id'] == "root" AND $_POST['input_mdp'] == "azerty")
    if($_POST['input_id'] == "admin" AND $_POST['input_mdp'] == "admin")
    {
      $_SESSION['state'] = 'connected';
      $_SESSION['erreur_connection'] = 0;
      header('Location: accueil.php');
    }
    else
    {
      $_SESSION['erreur_connection'] = 1;
    }
  } 
}
?>
<!doctype html>
<html lang="fr">
  <head>
    <meta charset="utf-8">
    <title>Projet securite</title>
    <link rel="stylesheet" href="dist/css/bootstrap.css">
    <link rel="stylesheet" href="css/style.css">
    <script src="dist/js/boostrap.js"></script>
  </head>
  <body>
    <div class="container" id="main_container">
      <h1 class="row centrage_index text-center">Projet Sécurité</h1>
      <h2 class="row centrage_index text-center">Supervision et Audit de la sécurité système dans un réseau</h2>
      <form class="" role="form" method="post" action="index.php">
	<div class="form-group">
	  <label class="text-center"for="input_id">Identifiant</label>
	  <input type="texte" class="form-control" id="input_id" name="input_id"/>
	</div>
	<div class="form-group">
	  <label class="text-center"for="input_mdp">Mot de passe</label>
	  <input type="password" class="form-control" id="input_mdp" name="input_mdp"/>
	</div>
	<button type="submit" class="btn btn-success">Connection</button>
      </form>
<?php 
if(isset($_SESSION['erreur_connection']) AND $_SESSION['erreur_connection']==1)
{?>
      <p class="lead text-center bg-danger">Identifiant ou mot de passe incorrect</p>
<?php } ?>
    </div>
<?php include 'include/partie/footer.php' ?>
  </body>
</html>
