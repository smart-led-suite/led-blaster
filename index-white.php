<html>
<head>
<meta charset="UTF-8" name="viewport" content="width=device-width" />
<title>LED Steuerung @ Home</title>
</head>
<body>
Licht an und ausschalten:
<form action="" method="get">
<table>
	<tr>
		<td> weiß
		<td> rot
		<td> grün
		<td> blau
	</tr>
	<tr>
		<td> <input type="radio" value="0" name="w">0% Helligkeit  <br>
		<td> <input type="radio" value="0" name="r">0% Helligkeit  <br>
		<td> <input type="radio" value="0" name="g">0% Helligkeit  <br>
		<td> <input type="radio" value="0" name="b">0% Helligkeit  <br>
	</tr>
	<tr>
  		<td> <input type="radio" value="50" name="w">50% Helligkeit <br>
  		<td> <input type="radio" value="50" name="r">50% Helligkeit <br>
  		<td> <input type="radio" value="50" name="g">50% Helligkeit <br>
  		<td> <input type="radio" value="50" name="b">50% Helligkeit <br>
  	</tr>
  	<tr>
  		<td> <input type="radio" value="100" name="w">100% Helligkeit <br> 
  		<td> <input type="radio" value="100" name="r">100% Helligkeit <br> 
  		<td> <input type="radio" value="100" name="g">100% Helligkeit <br> 
  		<td> <input type="radio" value="100" name="b">100% Helligkeit <br> 
	</tr>
	<tr>
  		<td> <input type="number" default="100" name="luminance_white" min="0" max="100">
  		<td> <input type="number" default="100" name="luminance_red" min="0" max="100">
  		<td> <input type="number" default="100" name="luminance_green" min="0" max="100">
  		<td> <input type="number" default="100" name="luminance_blue" min="0" max="100">
	</tr> 
</table>	
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

$luminanceWhite=$_GET['w'];  //wert für weiss einlesen
//$luminanceWhite=$white; //wert für weiss auf luminance geben
$luminanceRed=$_GET['r'];  //wert für weiss einlesen
//$luminanceWhite=$white; //wert für weiss auf luminance geben
$luminanceGreen=$_GET['g'];  //wert für weiss einlesen
//$luminanceWhite=$white; //wert für weiss auf luminance geben
$luminanceBlue=$_GET['b'];  //wert für weiss einlesen
//$luminanceWhite=$white; //wert für weiss auf luminance geben

if($fade == "") {
  $fade=1;  }   // default is on || immer an, da 0 in python noch nicht realisiert ist

$alternativeLuminanceWhite=$_GET['luminance_white'];
if($alternativeLuminanceWhite!="")  {
	$luminanceWhite=$_GET['luminance_white'];
}
$alternativeLuminanceRed=$_GET['luminance_red'];
if($alternativeLuminanceRed!="")  {
	$luminanceRed=$_GET['luminance_red'];
}
$alternativeLuminanceGreen=$_GET['luminance_green'];
if($alternativeLuminanceGreen!="")  {
	$luminanceGreen=$_GET['luminance_green'];
}
$alternativeLuminanceBlue=$_GET['luminance_blue'];
if($alternativeLuminanceBlue!="")  {
	$luminanceBlue=$_GET['luminance_blue'];
}

//echo $luminance;

echo $luminanceWhite;
 if($fade==1) {
    $cmd = "sudo ./universal.py 1 $luminanceWhite $luminanceRed $luminanceGreen $luminanceBlue"; 
    echo $cmd;
    $val =  shell_exec($cmd); 
    echo $val;
   echo "Licht ist aus";
    echo $luminanceWhite;
}
  else { 
    $cmd = "sudo ./universal.py 0 $luminanceWhite $luminanceRed $luminanceGreen $luminanceBlue"; 
    echo $cmd;
    $val =  shell_exec($cmd); 
    echo "Licht ist aus";  } 

?>
</body>
</html>
