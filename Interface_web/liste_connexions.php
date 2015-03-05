<?php
session_start();
if(isset($_SESSION['state'])) 
{
  if($_SESSION['state'] == 'new')
  {
    $_SESSION=array();
    session_destroy();
    header('Location: liste_connexions.php');
  }
}
else
{
  $_SESSION=array();
  session_destroy();
  header('Location: liste_connexions.php');
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

  $requete_info_connexions=$bdd->query("SELECT DISTINCT * FROM CONNEXIONS WHERE `CONNEXIONS`.`cli_id`='".$_GET['id']."' ORDER BY co_date_debut ASC");
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
				<h2 class="text-center">Liste des connexions sur le client ayant pour IP : <?php echo $info_clients['cli_ip'];?></h2>
			</br>
			</div>
		</div>

		<div class="row">
			<div class="row">
	  			<div class="table-responsive">
					<table class="table table-bordered table-hover table-condensed">
      			<thead>
					<tr>
						<th class="text-center">Nom utilisateur</th>
						<th class="text-center">Date de connexion</th>
						<th class="text-center">Date de déconnexion</th>
						<th class="text-center">Durée de connexions</th>
					</tr>
			    </thead>
			    <tbody>
				<?php 
				while( $liste_connexions=$requete_info_connexions->fetch() ) 
				{ 
				?>
					<tr>
						<td class="text-center"><?php echo $liste_connexions['co_utilisateur'];?></td>
						<td class="text-center"><?php echo $liste_connexions['co_date_debut'];?></td>
						<td class="text-center"><?php echo $liste_connexions['co_date_fin'];?></td>
						<?php
						list($heure,$minutes) = explode(':',$liste_connexions['co_duree']); 
						$duree = $heure.' heure(s) et '.$minutes.' minute(s)';
						?>
						<td class="text-center"><?php echo $duree;?></td>
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
