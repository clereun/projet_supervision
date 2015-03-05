<?php
session_start();
if(isset($_SESSION['state'])) 
{
  if($_SESSION['state'] == 'new')
  {
    $_SESSION=array();
    session_destroy();
    header('Location: liste_paquets_maj.php');
  }
}
else
{
  $_SESSION=array();
  session_destroy();
  header('Location: liste_paquets_maj.php');
}
include 'master.php';
if(isset($_GET['id']))
{
  try
  {
    $bdd = new PDO('mysql:host=localhost;dbname=PROJET_SECU;charset=utf8', $user_bdd,$mdp_bdd);
  }
  catch(Exception $e)
  {
    $_SESSION['msg_del']='Erreur : '.$e->getMessage();
  }

  $requete_info_clients=$bdd->query("SELECT * FROM CLIENTS WHERE `CLIENTS`.`cli_id`='".$_GET['id']."'");
  $info_clients=$requete_info_clients->fetch();

  $requete_info_paquets=$bdd->query("SELECT DISTINCT * FROM PAQUETS WHERE `PAQUETS`.`cli_id`='".$_GET['id']."' AND `PAQUETS`.`paq_version_installee` IS NOT NULL AND `PAQUETS`.`paq_date`= ( SELECT MAX(`paq_date`) FROM `PAQUETS` WHERE `cli_id`='".$_GET['id']."' ) ORDER BY paq_nom_programme ASC");
}
?>
<!doctype html>
<html lang="fr">
  <head>
    <meta charset="utf-8" />
    <title>Projet securite</title>
    <link rel="stylesheet" href="dist/css/bootstrap.css">
    <script src="dist/js/boostrap.js"></script>
  </head>

  <body>
    <div class="container">
    	<div class="row">
			<h4 class="text-right">
				<a href="traitement/deconnexion.php">Deconnexion</a>
			</h4>
			<h4 class="text-left">
				<a href="details.php?id=<?php echo $info_clients['cli_id'];?>" class="btn btn-xs btn-info" role="button">Retour vers informations client</a>
			</h4>
		</div>
		<div class="row">
			<h1 class="text-center">Supervision et Audit de la sécurité système dans un réseau</h1>
		</div>

      	<div class="row">
			<div class="row">
				<h2 class="text-center">Liste des paquets à mettre à jour sur le client ayant pour IP : <?php echo $info_clients['cli_ip'];?></h2>
			</br>
			</div>
		</div>

		<div class="row">
			<div class="row">
	  			<div class="table-responsive">
					<table class="table table-bordered table-hover table-condensed">
      			<thead>
					<tr>
						<th class="text-center">Date</th>
						<th class="text-center">Nom programme</th>
						<th class="text-center">Version installée</th>
						<th class="text-center">Version disponible</th>
					</tr>
			    </thead>
			    <tbody>
				<?php 
				while( $info_paquets=$requete_info_paquets->fetch() ) 
				{ 
				?>
					<tr>
						<td class="text-center"><?php echo date("d/m/Y",strtotime($info_paquets['paq_date']));?></td>
					  	<td class="text-center"><?php echo $info_paquets['paq_nom_programme'];?></td>
					  	<td class="text-center"><?php echo $info_paquets['paq_version_installee'];?></td>
						<td class="text-center"><?php echo $info_paquets['paq_version_maj'];?></td>
					</tr>
				<?php
				}
				?>
				</tbody>
			    </table>
			</div>
		</div>


    </div>
<?php include 'include/partie/footer.php';?>
</body>
</html>
