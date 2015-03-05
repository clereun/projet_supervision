<?php
  $requete_info_script_generale=$bdd->query("SELECT script_id FROM SCRIPT WHERE `SCRIPT`.`script_name`='script_0.sh';");
  $info_script_generale=$requete_info_script_generale->fetch();
?>
<div class="row">
	<div class="row">
		<div class="col-sm-12">
			<div class="col-sm-6 text-left">
				<h3 class="text-left">Informations générales</h3>
			</div>
			<div class="col-sm-6 text-right">
			<?php
			if($info_script_generale['script_id']!=NULL)
			{
				?>
				<a href="socket/exe.php?id=<?php echo $_GET['id'];?>&script=<?php echo $info_script_generale['script_id']; ?>" class="btn btn-primary">Scanner les informations générales</a>
				<?php
			}
			else			
			{
				?>
				<a href="#" class="btn btn-warning">Scanner les informations générales</a>
				<?php
			}
			?>
			</div>
		</div>
	</div>
	<div class="row">
		<div class="col-sm-12">
			<ul>
				<li>Hostname : <?php echo $info_clients['cli_hostname'];?></li>
				<?php
				if ( $info_clients['cli_distrib'] == "")
				{
				?>
				</ul>
					<div class="row">
						<div class="col-sm-12">
							<h4 class="text-left"><span style="color:red;">Vous ne disposez pas d'informations générales sur ce client. <br /> Veuillez proceder au scan.</span></h4>
						</div>
					</div>
				<?php
				}
				else
				{
				?>
					<li>Distribution : <?php echo $info_clients['cli_distrib'];?></li>
					<?php
					while ( $info_versions=$requete_info_versions->fetch() ) 
					{
					?>
						<li>A la date du : <?php echo date("d/m/Y",strtotime($info_versions['vers_date']));?></li>
						<ul>
							<li>Version logiciel de scan: <?php echo $info_versions['vers_logiciel'];?></li>
							<li>Version distribution client : <?php echo $info_versions['vers_distribution'];?></li>
							<li>Version noyau : <?php echo $info_versions['vers_noyau'];?></li>
						</ul>
					<?php
					}
				?>
			</ul>
			<?php
			}
			?>
		</div>
	</div>
</div>
