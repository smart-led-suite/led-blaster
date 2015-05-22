<html>
<head>
<meta charset="UTF-8" /> 
<meta name="viewport" content="width=device-width, initial-scale=1">

<title>LED Steuerung @ Home</title>
<link rel="icon" 
      type="image/ico" 
      href="favicon.ico">
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
		<td> <input type="radio" value="-1" name="w" checked>keine Änderung  <br>
		<td> <input type="radio" value="-1" name="r" checked>keine Änderung  <br>
		<td> <input type="radio" value="-1" name="g" checked>keine Änderung  <br>
		<td> <input type="radio" value="-1" name="b" checked>keine Änderung  <br>
	</tr>
	<tr>
		<td> <input type="radio" value="0" name="w">0% Helligkeit  <br>
		<td> <input type="radio" value="0" name="r">0% Helligkeit  <br>
		<td> <input type="radio" value="0" name="g">0% Helligkeit  <br>
		<td> <input type="radio" value="0" name="b">0% Helligkeit  <br>
	</tr>
	<tr>
  		<td> <input type="radio" value="25" name="w">25% Helligkeit <br>
  		<td> <input type="radio" value="25" name="r">25% Helligkeit <br>
  		<td> <input type="radio" value="25" name="g">25% Helligkeit <br>
  		<td> <input type="radio" value="25" name="b">25% Helligkeit <br>
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
	<tr> <!-- alternative luminances "a_" -->
  		<td> <input type="number" default="100" name="a_white" min="0" max="100">
  		<td> <input type="number" default="100" name="a_red" min="0" max="100">
  		<td> <input type="number" default="100" name="a_green" min="0" max="100">
  		<td> <input type="number" default="100" name="a_blue" min="0" max="100">
	</tr> 
</table>	
  <br> 
  <input type="radio" name="fade" value="0" checked>kein Fade
  <br>
  <input type="radio" name="fade" value="1" checked>Fade (einzeln)
  <br>
  <input type="radio" name="fade" value="2" checked>Fade (synchron)
  <br>
  <!--<input type="number" default="5" name="speed" min="1" max="49"> Speed (1=langsam, 49=schnell)-->
  <br>
  <input type="submit" value="LEDs schalten">
</form>
<br>

<br> <br>

<?php

$fade=$_GET['fade'];  //fade einlesen
//$speed=$_GET['speed'];  //fade einlesen

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
if($speed == "") {
  $speed=1;  }   // default is on || immer an, da 0 in python noch nicht realisiert ist


//wenn etwas in das Textfeld eingetragen wird, dann wird dieser wert der jeweiligen helligkeitFarbe zugeordnet
//das Textfeld überschreibt also die checkboxen
$alternativeWhite=$_GET['a_white'];
if($alternativeWhite!="")  {
	$luminanceWhite=$_GET['a_white'];
}
$alternativeRed=$_GET['a_red'];
if($alternativeRed!="")  {
	$luminanceRed=$_GET['a_red'];
}
$alternativeGreen=$_GET['a_green'];
if($alternativeGreen!="")  {
	$luminanceGreen=$_GET['a_green'];
}
$alternativeBlue=$_GET['a_blue'];
if($aalternativeBlue!="")  {
	$luminanceBlue=$_GET['a_blue'];
}

//echo $luminance;

//echo $luminanceWhite;
 if($fade==1) {
    $cmd = "./led-client.py 1 $speed $luminanceWhite $luminanceRed $luminanceGreen $luminanceBlue"; //print to python script
   $val =  shell_exec($cmd); 
}
elseif($fade==2) {
	    $cmd = "./led-client.py 2 $speed $luminanceWhite $luminanceRed $luminanceGreen $luminanceBlue"; //print to python script
	   $val =  shell_exec($cmd); 
}  else { 
    $cmd = "./led-client.py 0 $speed $luminanceWhite $luminanceRed $luminanceGreen $luminanceBlue";
    $val =  shell_exec($cmd);
    } 

?>
</body>
</html>
