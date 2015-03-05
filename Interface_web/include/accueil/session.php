<?php 
session_start();
if(isset($_SESSION['state'])) 
{
  if($_SESSION['state'] == 'new')
  {
    $_SESSION=array();
    session_destroy();
    header('Location: index.php');
  }
}
else
{
  $_SESSION=array();
  session_destroy();
  header('Location: index.php');
}
?>