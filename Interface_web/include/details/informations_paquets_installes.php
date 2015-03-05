<?php
  $requete_info_script_installe=$bdd->query("SELECT DISTINCT script_id FROM SCRIPT WHERE `SCRIPT`.`script_name`='script_4.sh'");
  $info_script_installe=$requete_info_script_installe->fetch();
?>
<div class="row">
	<div class="row">
		<div class="col-sm-12">
			<div class="col-sm-6 text-left">
				<h3 class="text-left">Liste des paquets installés</h3>
			</div>
			<div class="col-sm-6 text-right">
			<?php
			if($info_script_installe['script_id']!=NULL)
			{
				?>
				<a href="socket/exe.php?id=<?php echo $_GET['id'];?>&script=<?php echo $info_script_installe['script_id']; ?>" class="btn btn-primary">Scanner la liste des paquets installés</a>
				<?php
			}
			else			
			{
				?>
				<a href="#" class="btn btn-warning">Scanner la liste des paquets installés</a>
				<?php
			}
			?>
			</div>
		</div>
	</div>
	<div class="row">
	    <div class="col-sm-12">
	    <?php
		if ( ($liste_paquets=$requete_liste_paquets->fetch()) == FALSE )
		{
		?>
			<div class="row">
				<div class="col-sm-12">
					<h4 class="text-left"><span style="color:red;">Vous ne disposez pas d'informations sur les paquets installés sur ce client. <br /> Veuillez proceder au scan.</span></h4>
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
						<th class="text-center">Nom programme</th>
					</tr>
			    </thead>
			    <tbody>
				    <tr>
				      	<td class="text-center"><?php echo $liste_paquets['paq_nom_programme'];?></td>
				    </tr>
				<?php 
				$compteur = 0;
				while($liste_paquets=$requete_liste_paquets->fetch()) 
				{ 
				?>
					<tr>
				  		<td class="text-center"><?php echo $liste_paquets['paq_nom_programme'];?></td>
					</tr>
				<?php
					$compteur++;
				}
				if ( $compteur >= 9 ) 
				{
				?>
					<tr>
					  	<td class="text-center">
					  	<a href="liste_paquets.php?id=<?php echo $info_clients['cli_id'];?>" class="btn btn-xs btn-info" role="button" onClick="">Tous les paquets</a>
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
