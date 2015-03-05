<?php 
  $requete_info_script_routage=$bdd->query("SELECT script_id FROM SCRIPT WHERE `SCRIPT`.`script_name`='script_3.sh'");
  $info_script_routage=$requete_info_script_routage->fetch()

?>
<div class="row">
	<div class="row">
		<div class="col-sm-12">
		    <div class="row">
			<div class="col-sm-6 text-left">
				<h3 class="text-left">Table de routages</h3>
			</div>
			<div class="col-sm-6 text-right">
			<?php
			if( $info_script_routage['script_id'] <> NULL )
			{
				?>
				<a href="socket/exe.php?id=<?php echo $_GET['id'];?>&script=<?php echo $info_script_routage['script_id']; ?>" class="btn btn-primary">Scanner les informations liées à la table de routages</a>
				<?php
			}
			else			{
				?>
				<a href="#" class="btn btn-warning">Scanner les informations liées à la table de routages</a>
				<?php
			}
			?>
			</div>
		    </div>
		</div>
	</div>
	<div class="row">
	    <div class="col-sm-12">
	    <?php

		if ( ($info_routes=$requete_info_routes->fetch()) == FALSE )
		{
		?>
			<div class="row">
				<div class="col-sm-12">
					<h4 class="text-left"><span style="color:red;">Vous ne disposez pas d'informations sur la table de routage de ce client. <br /> Veuillez proceder au scan.</span></h4>
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
					  <th class="text-center">Destination</th>
					  <th class="text-center">Passerelle</th>
					  <th class="text-center">Masque</th>
					  <th class="text-center">Interface</th>
					</tr>
			    </thead>
			    <tbody>
			    <tr>
				  	<td class="text-center"><?php echo $info_routes['route_dest'];?></td>
				  	<td class="text-center"><?php echo $info_routes['route_passerelle'];?></td>
					<td class="text-center"><?php echo $info_routes['route_masque'];?></td>
					<td class="text-center"><?php echo $info_routes['route_interface'];?></td>
				</tr>
				<?php 
				while($info_routes=$requete_info_routes->fetch()) 
				{ 
				?>
					<tr>
					  	<td class="text-center"><?php echo $info_routes['route_dest'];?></td>
					  	<td class="text-center"><?php echo $info_routes['route_passerelle'];?></td>
						<td class="text-center"><?php echo $info_routes['route_masque'];?></td>
						<td class="text-center"><?php echo $info_routes['route_interface'];?></td>
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
