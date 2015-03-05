<?php
  $requete_info_script_ecoute=$bdd->query("SELECT DISTINCT script_id FROM SCRIPT WHERE `SCRIPT`.`script_name`='script_6.sh'");
  $info_script_ecoute=$requete_info_script_ecoute->fetch();
?>
<div class="row">
	<div class="row">
		<div class="col-sm-12">
			<div class="col-sm-6 text-left">
				<h3 class="text-left">Liste des services en écoute</h3>
			</div>
			<div class="col-sm-6 text-right">
			<?php
			if($info_script_ecoute['script_id']!=NULL)
			{
				?>
				<a href="socket/exe.php?id=<?php echo $_GET['id'];?>&script=<?php echo $info_script_ecoute['script_id']; ?>" class="btn btn-primary">Scanner les services en écoute</a>
				<?php
			}
			else			{
				?>
				<a href="#" class="btn btn-warning">Scanner les services en écoute</a>
				<?php
			}
			?>
			</div>
		</div>
	</div>
	<div class="row">
	    <div class="col-sm-12">
	    <?php
	    if ( ($liste_ecoute=$requete_info_ecoute->fetch()) == FALSE )
		{
		?>
			<div class="row">
				<div class="col-sm-12">
					<h4 class="text-left"><span style="color:red;">Vous ne disposez pas d'informations sur les services en écoute sur ce client. <br /> Veuillez proceder au scan.</span></h4>
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
						<th class="text-center">Nom service</th>
						<th class="text-center">PID</th>
						<th class="text-center">Adresse d'écoute</th>
						<th class="text-center">Port</th>
						<th class="text-center">Protocol</th>
						<th class="text-center">Adresse distante</th>
						<th class="text-center">Port distant</th>
					</tr>
			    </thead>
			    <tbody>
			    	<tr>
						<td class="text-center"><?php echo $liste_ecoute['listen_nom_prog'];?></td>
						<td class="text-center"><?php echo $liste_ecoute['listen_pid'];?></td>
						<td class="text-center"><?php echo $liste_ecoute['listen_adresse'];?></td>
						<td class="text-center"><?php echo $liste_ecoute['listen_port'];?></td>
						<td class="text-center"><?php echo $liste_ecoute['listen_protocol'];?></td>
						<td class="text-center"><?php 
												if ( $liste_ecoute['listen_adresse_distante'] == "" ) 
												{
													echo "Non renseignée";
												}
												else
												{
													echo $liste_ecoute['listen_adresse_distante'];
												}
												?>
						</td>
						<td class="text-center"><?php echo $liste_ecoute['listen_port_distant'];?></td>
					</tr>
				<?php
				$compteur=0; 
				while( $liste_ecoute=$requete_info_ecoute->fetch() ) 
				{ 
				?>
					<tr>
						<td class="text-center"><?php echo $liste_ecoute['listen_nom_prog'];?></td>
						<td class="text-center"><?php echo $liste_ecoute['listen_pid'];?></td>
						<td class="text-center"><?php echo $liste_ecoute['listen_adresse'];?></td>
						<td class="text-center"><?php echo $liste_ecoute['listen_port'];?></td>
						<td class="text-center"><?php echo $liste_ecoute['listen_protocol'];?></td>
						<td class="text-center"><?php 
												if ( $liste_ecoute['listen_adresse_distante'] == "" ) 
												{
													echo "Non renseignée";
												}
												else
												{
													echo $liste_ecoute['listen_adresse_distante'];
												}
												?>
						</td>
						<td class="text-center"><?php echo $liste_ecoute['listen_port_distant'];?></td>
					</tr>
				<?php
				$compteur++;
				}
				if ( $compteur >= 4 ) 
				{
				?>
					<tr>
					  	<td colspan="7" class="text-center">
					  	<a href="liste_ecoute.php?id=<?php echo $info_clients['cli_id'];?>" class="btn btn-xs btn-info" role="button" onClick="">Tous les services en écoute sur ce client</a>
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
