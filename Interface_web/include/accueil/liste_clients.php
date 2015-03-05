<div class="row">
    <div class="row">
	<h2 class="text-center">Liste des clients</h2>
    </div>
    <div class="row">
	<div class="table-responsive">
	    <table class="table table-bordered table-hover table-condensed">
		<thead>
		    <tr>
			<th class="text-center">ID</th>
			<th class="text-center">IP</th>
			<th class="text-center">Hostname</th>
			<th class="text-center">Version</th>
			<th class="text-center">Détails</th>
			<th class="text-center">Suppression</th>
		    </tr>
		</thead>
		<tbody>
<?php 
$reponse=$bdd->query("SELECT * FROM CLIENTS");
while($donnees=$reponse->fetch()) 
{ 
?>
		    <tr>
			<td class="text-center"><?php echo $donnees['cli_id'];?></td>
			<td class="text-center"><?php echo $donnees['cli_ip'];?></td>
<?php 
    if($donnees['cli_hostname']=="")
    {
?>
			<td class="text-center">NULL</td>
<?php 
    } 
    else 
    { 
?>
			<td class="text-center"><?php echo $donnees['cli_hostname'];?></td>
<?php 
    }  
    if($donnees['cli_distrib']=="")
    {
?>
			<td class="text-center">NULL</td>
<?php
    } 
    else 
    { 
?>
			<td class="text-center"><?php echo $donnees['cli_distrib'];?></td>
<?php 
    } 
?>
			<td class="text-center">
			    <a href="details.php?id=<?php echo $donnees['cli_id'];?>" class="btn btn-xs btn-info" role="button">détails</a>
			</td>
			<td class="text-center">
			    <a href="traitement/del_entree.php?id=<?php echo $donnees['cli_id'];?>" class="btn btn-xs btn-danger" role="button" onclick="return confirm('Etes vous sûre de vouloir supprimer cette entrée ?');">Supprimer</a>
			</td>
		    </tr>
<?php
}
?>
		</tbody>
	    </table>
	</div>
    </div>
</div>
