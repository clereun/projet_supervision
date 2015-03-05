<?php
include("include/accueil/session.php");
include("master.php");
include("connection_bdd.php");
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
	    <?php include("include/top_boutons/top_boutons_accueil.php"); ?>
	    <div class="row">
		<h1 class="row centrage_accueil text-center">Supervision et Audit de la sécurité système dans un réseau</h1>
	    </div>
<?php 
include("include/accueil/ajout_client.php"); 
include("include/accueil/liste_clients.php"); 
?>      	
	</div>
	<?php include 'include/partie/footer.php';?>
    </body>
</html>
