<?php
  $requete_info_script_utilisateur=$bdd->query("SELECT DISTINCT script_id FROM SCRIPT WHERE `SCRIPT`.`script_name`='script_1.sh'");
  $info_script_utilisateur=$requete_info_script_utilisateur->fetch();
?>
<div class="row">
	<div class="row">
		<div class="col-sm-12">
			<div class="col-sm-6 text-left">
				<h3 class="text-left">Informations sur les utilisateurs</h3>
			</div>
			<div class="col-sm-6 text-right">
			<?php
			if($info_script_utilisateur['script_id']!=NULL)
			{
				?>
				<a href="socket/exe.php?id=<?php echo $_GET['id'];?>&script=<?php echo $info_script_utilisateur['script_id']; ?>" class="btn btn-primary">Scanner les informations sur les utilisateurs</a>
				<?php
			}
			else			{
				?>
				<a href="#" class="btn btn-warning">Scanner les informations sur les utilisateurs</a>
				<?php
			}
			?>
			</div>
		</div>
	</div>
	<div class="row">
		<div class="col-sm-12">
		<?php
		if ( ($info_utilisateurs=$requete_info_utilisateurs->fetch()) == FALSE )
		{
		?>
			<div class="row">
				<div class="col-sm-12">
					<h4 class="text-left"><span style="color:red;">Vous ne disposez pas d'informations sur les utilisateurs de ce client. <br /> Veuillez proceder au scan.</span></h4>
				</div>
			</div>
		<?php
		}
		else
		{
		?>
			<ul>
				<li>Nom : <?php echo $info_utilisateurs['util_nom'];?></li>
				<li>UID : <?php echo $info_utilisateurs['util_uid'];?></li>
				<li>Appartient au groupe : <?php echo $info_utilisateurs['util_groupe'];?></li>
				<li>Son mot de passe a été testé par John the ripper: 
					<?php 
					if ( $info_utilisateurs['util_john'] == 0 ) 
					{
						echo "Non";
					}
					else
					{
						echo "Oui";
					}
					?>
				</li>
				<li>Mot de passe robuste : 
					<?php 
					if ( $info_utilisateurs['util_passwd_fort'] == 0 ) 
					{
						echo "Non";
					}
					else
					{
						echo "Oui";
					}
					?>
				</li>
			</ul>
			<?php
			while ( $info_utilisateurs=$requete_info_utilisateurs->fetch() ) 
			{
			?>
			<ul>
				<li>Nom : <?php echo $info_utilisateurs['util_nom'];?></li>
				<li>UID : <?php echo $info_utilisateurs['util_uid'];?></li>
				<li>Appartient au groupe : <?php echo $info_utilisateurs['util_groupe'];?></li>
				<li>Son mot de passe a été testé par John the ripper: 
					<?php 
					if ( $info_utilisateurs['util_john'] == 0 ) 
					{
						echo "Non";
					}
					else
					{
						echo "Oui";
					}
					?>
				</li>
				<li>Mot de passe robuste : 
					<?php 
					if ( $info_utilisateurs['util_passwd_fort'] == 0 ) 
					{
						echo "Non";
					}
					else
					{
						echo "Oui";
					}
					?>
				</li>
			</ul>
			<?php
			} // FIN while utilisateurs
		}
		?>
		</div>
	</div>
</div>
