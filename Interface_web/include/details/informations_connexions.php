<?php
  $requete_info_script_connexions=$bdd->query("SELECT DISTINCT script_id FROM SCRIPT WHERE `SCRIPT`.`script_name`='script_2.sh'");
  $info_script_connexions=$requete_info_script_connexions->fetch();
?>
<div class="row">
	<div class="row">
		<div class="col-sm-12">
			<div class="col-sm-6 text-left">
				<h3 class="text-left">Liste des connexions utilisateurs</h3>
			</div>
			<div class="col-sm-6 text-right">
			<?php
			if($info_script_connexions['script_id']!=NULL)
			{
				?>
				<a href="socket/exe.php?id=<?php echo $_GET['id'];?>&script=<?php echo $info_script_connexions['script_id']; ?>" class="btn btn-primary">Scanner les connexions utilisateurs</a>
				<?php
			}
			else			{
				?>
				<a href="#" class="btn btn-warning">Scanner les connexions utilisateurs</a>
				<?php
			}
			?>
			</div>
		</div>
	</div>
	<div class="row">
	    <div class="col-sm-12">
	    <?php
	    if ( ($liste_connexions=$requete_info_connexions->fetch()) == FALSE )
		{
		?>
			<div class="row">
				<div class="col-sm-12">
					<h4 class="text-left"><span style="color:red;">Vous ne disposez pas d'informations sur les connexions sur ce client. <br /> Veuillez proceder au scan.</span></h4>
				</div>
			</div>
		<?php
		}
		else
		{
		?>
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
				$compteur=0; 
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
				$compteur++;
				}
				if ( $compteur >= 1 ) 
				{
				?>
					<tr>
					  	<td colspan="7" class="text-center">
					  	<a href="liste_connexions.php?id=<?php echo $info_clients['cli_id'];?>" class="btn btn-xs btn-info" role="button" onClick="">Toutes les connexions sur ce client</a>
					  	</td>
					</tr>
				<?php
				}
				?>
				</tbody>
			    </table>
			</div>
		<?php
		}
		?>
		</div>
	</div>
</div>
