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
  <input type="radio" name="mode" value="0" checked>Fade zu oben definierten Farben
  <br>
  <input type="radio" name="mode" value="1" >dauerhafter FadeMode 1 (WRGB an/aus)
  <br>
  <input type="number" default="1" name="speed" min="1" max="100"> Speed-Variable für Mode1 (Faktor, je höher desto langsamer der Fade)
  <br>
  <input type="radio" name="mode" value="2" >dauerhafter FadeMode 2 (von zufallsfarbe zu zufallsfarbe)
  <br>
  <input type="number" default="1" name="time" min="1" max="25000">Time Variable von Mode2, 1000ms = 1s bis nächste Farbe erreicht
  <br>
  <!--<input type="number" default="5" name="speed" min="1" max="49"> Speed (1=langsam, 49=schnell)-->
  <br>
  <input type="submit" value="LEDs schalten">
</form>
<br>

<br> <br>

<?php

$mode=$_GET['mode'];  //fade einlesen
$speed=$_GET['speed'];  //speed einlesen
$time=$_GET['time']; 	//time einlesen

$luminanceWhite=$_GET['w'];  //wert für weiss einlesen
//$luminanceWhite=$white; //wert für weiss auf luminance geben
$luminanceRed=$_GET['r'];  //wert für weiss einlesen
//$luminanceWhite=$white; //wert für weiss auf luminance geben
$luminanceGreen=$_GET['g'];  //wert für weiss einlesen
//$luminanceWhite=$white; //wert für weiss auf luminance geben
$luminanceBlue=$_GET['b'];  //wert für weiss einlesen
//$luminanceWhite=$white; //wert für weiss auf luminance geben

if($speed == "") 
{
  	$speed=1;     // default is factor 1
}
if($time == "") 
{
  	$time=1000;     // default is time=1000
}

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
if($alternativeBlue!="")  {
	$luminanceBlue=$_GET['a_blue'];
}

echo "alternativeBlue: " . $alternativeBlue . "<br> luminanceBlue: " . $luminanceBlue . "<br>";

$numberOfChangedBrightness = 0;
if ($luminanceWhite != -1) 
{
	$numberOfChangedBrightness++;
	$luminanceWhite = $luminanceWhite * 10;
}
if ($luminanceRed != -1) 
{
	$numberOfChangedBrightness++;
	$luminanceRed = $luminanceRed * 10;
}
if ($luminanceGreen != -1) 
{
	$numberOfChangedBrightness++;
	$luminanceGreen = $luminanceGreen * 10;
}
if ($luminanceBlue != -1) 
{
	$numberOfChangedBrightness++;
	$luminanceBlue = $luminanceBlue * 10;
}


//echo $luminance;

//echo $luminanceWhite;
if($mode==0) {								//fade to desired color/brightness
	$cmd = "echo mode=0 > /dev/led-blaster"; 			//enter mode 0
	$val =  shell_exec($cmd); 
	echo $cmd . "<br>";							//debugging info (only used at the beginning)
	$cmd = "echo wait=$numberOfChangedBrightness > /dev/led-blaster"; 			//set wait counter to $numberOfChangedBrightness (itll fade after changing four colorBrightnesses if everything was changed
	$val =  shell_exec($cmd); 
	echo $cmd . "<br>";							//debugging info (only used at the beginning)
	if ($luminanceWhite != -1)
	{
		$cmd = "echo w=$luminanceWhite > /dev/led-blaster"; 	//set each brightness WRGB
		$val =  shell_exec($cmd); 
		echo $cmd . "<br>\n";							//debugging info (only used at the beginning)
	}
	if ($luminanceRed != -1)
	{
		$cmd = "echo r=$luminanceRed > /dev/led-blaster"; 		//set each brightness WRGB
		$val =  shell_exec($cmd); 
		echo $cmd . "<br>\n";
	}							//debugging info (only used at the beginning)
	if ($luminanceGreen != -1)
	{
		$cmd = "echo g=$luminanceGreen > /dev/led-blaster"; 	//set each brightness WRGB
		$val =  shell_exec($cmd); 
		echo $cmd . "<br>\n";
		}
	if ($luminanceBlue != -1)
	{								//debugging info (only used at the beginning)
		$cmd = "echo b=$luminanceBlue > /dev/led-blaster"; 	//set each brightness WRGB
		$val =  shell_exec($cmd); 
		echo $cmd . "<br>\n";							//debugging info (only used at the beginning)
	}
}

else
{
	$cmd = "echo mode=$mode > /dev/led-blaster"; //echo the desired mode into led-blaster, thats all we have to do
	$val =  shell_exec($cmd); 
	echo $cmd . "<br>\n";							//debugging info (only used at the beginning)
	if ($mode == 1)
	{
		$cmd = "echo speed=$speed > /dev/led-blaster"; //echo the desired mode into led-blaster, thats all we have to do
		$val =  shell_exec($cmd); 
		echo $cmd . "<br>\n";							//debugging info (only used at the beginning)
	}
	else if ($mode == 2)
	{	
		$cmd = "echo time=$time > /dev/led-blaster"; //echo the desired mode into led-blaster, thats all we have to do
		$val =  shell_exec($cmd); 
		echo $cmd . "<br>\n";							//debugging info (only used at the beginning)
	}
}

?>
</body>
</html>
