<?php 
  $requete_info_script_securite=$bdd->query("SELECT DISTINCT script_id FROM SCRIPT WHERE `SCRIPT`.`script_name`='script_7.sh'");
  $info_script_securite=$requete_info_script_securite->fetch();
?>
<div class="row">
	<div class="row">
		<div class="col-sm-12">
		    <div class="row">
			<div class="col-sm-6 text-left">
				<h3 class="text-left">Informations de sécurité</h3>
			</div>
			<div class="col-sm-6 text-right">
			<?php
			if($info_script_securite['script_id']!=NULL)
			{
				?>
				<a href="socket/exe.php?id=<?php echo $_GET['id'];?>&script=<?php echo $info_script_securite['script_id']; ?>" onclick="return confirm('Etes vous sûre de vouloir scanner ces informations ? Vous risquez de perdre des informations si vous n'avez pas vérifié l'état des dossiers dont les hash sont differents.);"class="btn btn-primary">Scanner les informations liées à la table de routages</a>
				<?php
			}
			else			
			{
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
		if ( $info_dernier_scan_hd['hd_bin'] == "")
		{
		?>
			<div class="row">
				<div class="col-sm-12">
					<h4 class="text-left"><span style="color:red;">Vous ne disposez pas d'informations sur la sécurité de ce client. <br /> Veuillez proceder au scan.</span></h4>
				</div>
			</div>
		<?php
		}
		else
		{
		$compteur_defaut_hash=0;
		?>
			<ul>
				<?php
				if ( $info_dernier_scan['secu_shadow'] == $info_avant_dernier_scan['secu_shadow'] ) 
				{
				?>
				<li>Le dossier /etc/shadow n'a pas été modifié entre le <?php echo date("d/m/Y",strtotime($info_avant_dernier_scan['secu_date']));?> et le <?php echo date("d/m/Y",strtotime($info_dernier_scan['secu_date']));?></li>
				<?php
				}
				else
				{
				?>
				<li>Attention le dossier /etc/shadow a été modifié entre le <?php echo date("d/m/Y",strtotime($info_avant_dernier_scan['secu_date']));?> et le <?php echo date("d/m/Y",strtotime($info_dernier_scan['secu_date']));?></br><span style="color:red;"> Pensez à le vérifier</span></li>
				<?php
				$compteur_defaut_hash++;
				}
				if ( $info_dernier_scan['secu_passwd'] == $info_avant_dernier_scan['secu_passwd'] ) 
				{
				?>
				<li>Le dossier /etc/passwd n'a pas été modifié entre le <?php echo date("d/m/Y",strtotime($info_avant_dernier_scan['secu_date']));?> et le <?php echo date("d/m/Y",strtotime($info_dernier_scan['secu_date']));?></li>
				<?php
				}
				else
				{
				?>
				<li>Attention le dossier /etc/passwd a été modifié entre le <?php echo date("d/m/Y",strtotime($info_avant_dernier_scan['secu_date']));?> et le <?php echo date("d/m/Y",strtotime($info_dernier_scan['secu_date']));?></br><span style="color:red;"> Pensez à le vérifier</span></li>
				<?php
				$compteur_defaut_hash++;
				}
				?>
				<li>Le dernier scan de John the ripper a été fait le <?php echo date("d/m/Y",strtotime($info_dernier_scan['secu_date']));?></li>
				<?php
				if ( $info_dernier_scan_hd['hd_bin'] == $info_avant_dernier_scan_hd['hd_bin'] ) 
				{
				?>
				<li>Le dossier /bin n'a pas été modifié entre le <?php echo date("d/m/Y",strtotime($info_avant_dernier_scan_hd['hd_date']));?> et le <?php echo date("d/m/Y",strtotime($info_dernier_scan_hd['hd_date']));?></li>
				<?php
				}
				else
				{
				?>
				<li>Attention le dossier /bin a été modifié entre le <?php echo date("d/m/Y",strtotime($info_avant_dernier_scan_hd['hd_date']));?> et le <?php echo date("d/m/Y",strtotime($info_dernier_scan_hd['hd_date']));?></br><span style="color:red;"> Pensez à le vérifier</span></li>
				<?php
				$compteur_defaut_hash++;
				}
				if ( $info_dernier_scan_hd['hd_sbin'] == $info_avant_dernier_scan_hd['hd_sbin'] ) 
				{
				?>
				<li>Le dossier /sbin n'a pas été modifié entre le <?php echo date("d/m/Y",strtotime($info_avant_dernier_scan_hd['hd_date']));?> et le <?php echo date("d/m/Y",strtotime($info_dernier_scan_hd['hd_date']));?></li>
				<?php
				}
				else
				{
				?>
				<li>Attention le dossier /sbin a été modifié entre le <?php echo date("d/m/Y",strtotime($info_avant_dernier_scan_hd['hd_date']));?> et le <?php echo date("d/m/Y",strtotime($info_dernier_scan_hd['hd_date']));?></br><span style="color:red;"> Pensez à le vérifier</span></li>
				<?php
				$compteur_defaut_hash++;
				}
				if ( $info_dernier_scan_hd['hd_usr_bin'] == $info_avant_dernier_scan_hd['hd_usr_bin'] ) 
				{
				?>
				<li>Le dossier /usr/bin n'a pas été modifié entre le <?php echo date("d/m/Y",strtotime($info_avant_dernier_scan_hd['hd_date']));?> et le <?php echo date("d/m/Y",strtotime($info_dernier_scan_hd['hd_date']));?></li>
				<?php
				}
				else
				{
				?>
				<li>Attention le dossier /usr/bin a été modifié entre le <?php echo date("d/m/Y",strtotime($info_avant_dernier_scan_hd['hd_date']));?> et le <?php echo date("d/m/Y",strtotime($info_dernier_scan_hd['hd_date']));?></br><span style="color:red;"> Pensez à le vérifier</span></li>
				<?php
				$compteur_defaut_hash++;
				}
				if ( $info_dernier_scan_hd['hd_usr_sbin'] == $info_avant_dernier_scan_hd['hd_usr_sbin'] ) 
				{
				?>
				<li>Le dossier /usr/sbin n'a pas été modifié entre le <?php echo date("d/m/Y",strtotime($info_avant_dernier_scan_hd['hd_date']));?> et le <?php echo date("d/m/Y",strtotime($info_dernier_scan_hd['hd_date']));?></li>
				<?php
				}
				else
				{
				?>
				<li>Attention le dossier /usr/sbin a été modifié entre le <?php echo date("d/m/Y",strtotime($info_avant_dernier_scan_hd['hd_date']));?> et le <?php echo date("d/m/Y",strtotime($info_dernier_scan_hd['hd_date']));?></br><span style="color:red;"> Pensez à le vérifier</span></li>
				<?php
				$compteur_defaut_hash++;
				}
				if ( $info_dernier_scan_hd['secu_iptables'] == $info_avant_dernier_scan_hd['secu_iptables'] ) 
				{
				?>
				<li>Le fichier iptables n'a pas été modifié entre le <?php echo date("d/m/Y",strtotime($info_avant_dernier_scan['secu_date']));?> et le <?php echo date("d/m/Y",strtotime($info_dernier_scan['secu_date']));?></li>
				<?php
				}
				else
				{
				?>
				<li>Attention le fichier iptables a été modifié entre le <?php echo date("d/m/Y",strtotime($info_avant_dernier_scan['secu_date']));?> et le <?php echo date("d/m/Y",strtotime($info_dernier_scan['secu_date']));?></br><span style="color:red;"> Pensez à le vérifier</span></li>
				<?php
				$compteur_defaut_hash++;
				}
				?>
			</ul>
		<?php
		}
		?>
		</div>
	</div>
		</div>