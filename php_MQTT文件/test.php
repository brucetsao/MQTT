<?php require_once('Connections/connSQL.php'); ?>
<?php
if (!function_exists("GetSQLValueString")) {
function GetSQLValueString($theValue, $theType, $theDefinedValue = "", $theNotDefinedValue = "") 
{
  if (PHP_VERSION < 6) {
    $theValue = get_magic_quotes_gpc() ? stripslashes($theValue) : $theValue;
  }

  $theValue = function_exists("mysql_real_escape_string") ? mysql_real_escape_string($theValue) : mysql_escape_string($theValue);

  switch ($theType) {
    case "text":
      $theValue = ($theValue != "") ? "'" . $theValue . "'" : "NULL";
      break;    
    case "long":
    case "int":
      $theValue = ($theValue != "") ? intval($theValue) : "NULL";
      break;
    case "double":
      $theValue = ($theValue != "") ? doubleval($theValue) : "NULL";
      break;
    case "date":
      $theValue = ($theValue != "") ? "'" . $theValue . "'" : "NULL";
      break;
    case "defined":
      $theValue = ($theValue != "") ? $theDefinedValue : $theNotDefinedValue;
      break;
  }
  return $theValue;
}
}

mysql_select_db($database_connSQL, $connSQL);
$query_Recordset1 = "SELECT * FROM DataType";
$Recordset1 = mysql_query($query_Recordset1, $connSQL) or die(mysql_error());
$row_Recordset1 = mysql_fetch_assoc($Recordset1);
$totalRows_Recordset1 = mysql_num_rows($Recordset1);
?>


<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />

<title>無標題文件</title>
</head>

<body>
<p>PHP Form 練習</p>
<p>
  <?php



require("phpMQTT-master/phpMQTT.php");

$server = "";     // change if necessary
$port = 1883;                     // change if necessary
$username = "";                   // set your username
$password = "";                   // set your password
$client_id = ""; // make sure this is unique for connecting to sever - you could use uniqid()
echo "ready";
$mqtt = new Bluerhinos\phpMQTT($server, $port, $client_id);
echo "set";
if ($mqtt->connect(true, NULL, $username, $password)) {
	echo "connect";
	$mqtt->publish("publishtest", "Hello World! ", 0);
	$mqtt->close();
	echo "SUCCESS!\n";
} else {
    echo "Time out!\n";
}



?>

</p>

</body>
</html>
<?php
mysql_free_result($Recordset1);
?>
