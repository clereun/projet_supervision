<?php
  $requete_info_script_paq_maj=$bdd->query("SELECT DISTINCT script_id FROM SCRIPT WHERE `SCRIPT`.`script_name`='script_5.sh'");
  $info_script_paq_maj=$requete_info_script_paq_maj->fetch();
?>
<div class="row">
	<div class="row">
		<div class="col-sm-12">
			<div class="col-sm-6 text-left">
				<h3 class="text-left">Liste des paquets à mettre à jour</h3>
			</div>
			<div class="col-sm-6 text-right">
			<?php
			if($info_script_paq_maj['script_id']!=NULL)
			{
				?>
				<a href="socket/exe.php?id=<?php echo $_GET['id'];?>&script=<?php echo $info_script_maj['script_id']; ?>" class="btn btn-primary">Scanner les paquets à mettre à jour</a>
				<?php
			}
			else			{
				?>
				<a href="#" class="btn btn-warning">Scanner les paquets à mettre à jour</a>
				<?php
			}
			?>
			</div>
		</div>
	</div>
	<div class="row">
	    <div class="col-sm-12">
	    <?php
	    if ( ($info_paquets=$requete_info_paquets->fetch()) == FALSE )
		{
		?>
			<div class="row">
				<div class="col-sm-12">
					<h4 class="text-left">Tous les paquets sur ce client semblent à jour.</h4>
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
						<th class="text-center">Date</th>
						<th class="text-center">Nom programme</th>
						<th class="text-center">Version installée</th>
						<th class="text-center">Version disponible</th>
					</tr>
			    </thead>
			    <tbody>
			    	<tr>
						<td class="text-center"><?php echo date("d/m/Y",strtotime($info_paquets['paq_date']));?></td>
					  	<td class="text-center"><?php echo $info_paquets['paq_nom_programme'];?></td>
					  	<td class="text-center"><?php echo $info_paquets['paq_version_installee'];?></td>
						<td class="text-center"><?php echo $info_paquets['paq_version_maj'];?></td>
					</tr>
				<?php 
				$compteur=0;
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
				$compteur++;
				}
				if ( $compteur >= 9 ) 
				{
				?>
					<tr>
					  	<td colspan="4" class="text-center">
					  	<a href="liste_paquets_maj.php?id=<?php echo $info_clients['cli_id'];?>" class="btn btn-xs btn-info" role="button" onClick="">Tous les paquets</a>
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
