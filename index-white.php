<html>
<head>
<meta name="viewport" content="width=device-width" />
<title>LED Steuerung @ Home</title>
</head>
<body>
Licht an und ausschalten:
<form action="" method="get">
  <input type="radio" value="0" name="w">0% Helligkeit  <br>
  <input type="radio" value="50" name="w">50% Helligkeit <br>
  <input type="radio" value="100" name="w">100% Helligkeit <br> 
  targetLuminance eingeben: 0 bis 100: 
  <input type="number" default="100" name="luminance" min="0" max="100">
  <br> 
  <input type="radio" name="fade" value="0" checked>kein Fade
  <br>
  <input type="radio" name="fade" value="1" checked>Fade
  <br>
  <input type="submit" name="submit_l">
</form>
<br>




immer erst ausschalten, wenn 'Licht ist an' dasteht und umgekehrt! <br>
nach druecken des Knopfes WARTEN bis die Aktion abgeschlossen ist und das Ergebnis 'Licht ist aus/an' angezeigt wird!
<br> <br>

<?php

$fade=$_GET['fade'];  //fade einlesen
$white=$_GET['w'];  //wert für weiss einlesen
$luminance=$white; //wert für weiss auf luminance geben

if($fade == "") {
  $fade=1;  }   // default is on || immer an, da 0 in python noch nicht realisiert ist

$alternativeLuminance=$_GET['luminance'];
if($alternativeLuminance!="")  {
	$luminance=$_GET['luminance'];
}
//echo $luminance;

echo $luminance;
 if($fade==1) {
    $cmd = "sudo ./universal.py 1 $luminance 0 0 0"; 
    echo $cmd;
    $val =  shell_exec($cmd); 
    echo $val;
   echo "Licht ist aus";
    echo $luminance;
}
  else { 
    $cmd = "sudo ./universal.py 1 $luminance 0 0 0"; 
    echo $cmd;
    $val =  shell_exec($cmd); 
    echo "Licht ist aus";  } 

?>
</body>
</html>
