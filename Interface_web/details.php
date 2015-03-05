<?php
include("include/details/session.php");
include("master.php");
if(isset($_GET['id']))
{
    include("connection_bdd.php");
    include("include/requetes/requetes.php");
}
?>
<!doctype html>
<html lang="fr">
  <head>
    <meta http-equiv="content-type" content="text/html" charset="utf-8" />
    <title>Projet securite</title>
    <link rel="stylesheet" href="dist/css/bootstrap.css">
    <link rel="stylesheet" href="css/style.css">
    <script src="dist/js/boostrap.js"></script>
  </head>

  <body>
    <div class="container">
	<?php include("include/top_boutons/top_boutons_details.php");?>
		<div class="row">
			<div class="col-sm-12">
				<h1 class="row centrage_accueil text-center">Supervision et Audit de la sécurité système dans un réseau</h1>
			</div>
		</div>
		<?php
		if ( $info_clients['cli_registered'] == "0" ) 
		{
		?>
		<div class="row centrage">
		    <div class="col-sm-8 col-sm-offset-2">
			<div class="row">
				<h1 class="text-center">Ce client n'est pas encore enregistré sur le serveur</h1>
				</div>
				<div class="row">
					<h3 class="text-center">Veuillez effectuer un enregistrement avant d'acceder à ces opération</h3>
				</div>			
				<div class="text-center">
				<a href="socket/register.php?id=<?php echo $_GET['id'];?>" class="btn btn-primary btn-lg">Enregistrer le client</a>
				</div>
		    </div>
		</div>
	</div>
<?php
}
else
{
?>

	<div class="row">
			<div class="row">
				<h2 class="row centrage_details text-center">Informations sur le client ayant pour IP : <?php echo $info_clients['cli_ip'];?></h2>
			</div>
		</div>
<?php 
    include("include/details/informations_generales.php"); 
    include("include/details/informations_utilisateurs.php"); 
    include("include/details/informations_securites.php"); 
    include("include/details/informations_route.php"); 
    include("include/details/informations_paquets_installes.php"); 
    include("include/details/informations_paquets_maj.php"); 
    include("include/details/informations_ecoute.php"); 
    include("include/details/informations_connexions.php"); 

} //  FIN du else si client possèdes des info dans la bdd
?>

    </div>
<?php include 'include/partie/footer.php';?>
</body>
</html>
