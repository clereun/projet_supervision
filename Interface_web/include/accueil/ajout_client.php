<div class="row">
	<div class="row">
			<h2 class="text-center">Ajout d'un client</h2>
	</div>
	<div class="row">
			<form role="form" method="post" action="traitement/add_entree.php">
    		<div class="form-group">
      			<label for="input_ip">Adresse IPv4 du nouveau Client</label>
    		</div>
    		<div class="form-group">
      			<input type="text" class="form-control" id="input_ip" name="input_ip" placeholder="Entrer l'IP du nouveau client"/>
    		</div>
    		<div class="form-group">
      			<button type="submit" class="btn btn-success">Ajouter</button>
    		</div>
			</form>
	</div>
	<?php 
	if(isset($_SESSION['msg_format']) AND $_SESSION['msg_format']==1) 
	{ 
	?>
	<div class="text-center">
	  	<p class="col-sm-6 col-sm-offset-3 lead text-center bg-danger">La chaine rentré n'est pas une adresse IP valide</p>
	</div>
	<?php 
	}
	if(isset($_SESSION['msg_taille_ip']) AND $_SESSION['msg_taille_ip']==1) 
	{ 
	?>
	<div class="text-center">
	 	<p class="col-sm-6 col-sm-offset-3 lead text-center bg-danger">La chaine rentré est vide</p>
	</div>
	<?php 
	} 
	?>
</div>