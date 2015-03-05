<?php
  $requete_info_clients=$bdd->query("SELECT DISTINCT * FROM CLIENTS WHERE `CLIENTS`.`cli_id`='".$_GET['id']."' AND `CLIENTS`.`cli_date`= ( SELECT MAX(`cli_date`) FROM `CLIENTS` WHERE `cli_id`='".$_GET['id']."' ) ");
  $info_clients=$requete_info_clients->fetch();

  $requete_info_versions=$bdd->query("SELECT DISTINCT * FROM VERSIONS WHERE `VERSIONS`.`cli_id`='".$_GET['id']."' AND `VERSIONS`.`vers_date`= ( SELECT MAX(`vers_date`) FROM `VERSIONS` WHERE `cli_id`='".$_GET['id']."' ) ORDER BY vers_id ASC");

  $requete_info_utilisateurs=$bdd->query("SELECT DISTINCT * FROM UTILISATEURS WHERE `UTILISATEURS`.`cli_id`='".$_GET['id']."' AND `UTILISATEURS`.`util_date`= ( SELECT MAX(`util_date`) FROM `UTILISATEURS` WHERE `cli_id`='".$_GET['id']."' ) ORDER BY util_id ASC");

  $requete_info_securite=$bdd->query("SELECT DISTINCT * FROM `SECURITE` WHERE cli_id='".$_GET['id']."' order by secu_id desc limit 0, 2");
  $info_dernier_scan=$requete_info_securite->fetch();
  $info_avant_dernier_scan=$requete_info_securite->fetch();

  $requete_info_hd=$bdd->query("SELECT DISTINCT * FROM `HASH_DOSSIERS` WHERE cli_id='".$_GET['id']."' order by hd_id desc limit 0, 2");
  $info_dernier_scan_hd=$requete_info_hd->fetch();
  $info_avant_dernier_scan_hd=$requete_info_hd->fetch();

  $requete_info_routes=$bdd->query("SELECT DISTINCT * FROM ROUTES WHERE `ROUTES`.`cli_id`='".$_GET['id']."' AND `ROUTES`.`route_date`= ( SELECT MAX(`route_date`) FROM `ROUTES` WHERE `cli_id`='".$_GET['id']."' ) ORDER BY route_id ASC");

  $requete_liste_paquets=$bdd->query("SELECT DISTINCT paq_nom_programme FROM PAQUETS WHERE `PAQUETS`.`cli_id`='".$_GET['id']."' AND `PAQUETS`.`paq_date`= ( SELECT MAX(`paq_date`) FROM `PAQUETS` WHERE `cli_id`='".$_GET['id']."' ) ORDER BY paq_nom_programme ASC limit 10");

  $requete_info_paquets=$bdd->query("SELECT DISTINCT * FROM PAQUETS WHERE `PAQUETS`.`cli_id`='".$_GET['id']."' AND `PAQUETS`.`paq_version_installee` IS NOT NULL AND `PAQUETS`.`paq_date`= ( SELECT MAX(`paq_date`) FROM `PAQUETS` WHERE `cli_id`='".$_GET['id']."' ) ORDER BY paq_nom_programme ASC limit 10");

  $requete_info_ecoute=$bdd->query("SELECT DISTINCT * FROM LISTEN WHERE `LISTEN`.`cli_id`='".$_GET['id']."' AND `LISTEN`.`listen_date`= ( SELECT MAX(`listen_date`) FROM `LISTEN` WHERE `cli_id`='".$_GET['id']."' ) ORDER BY listen_port ASC limit 5");

  $requete_info_connexions=$bdd->query("SELECT DISTINCT * FROM CONNEXIONS WHERE `CONNEXIONS`.`cli_id`='".$_GET['id']."' ORDER BY co_date_debut ASC limit 10");
?>