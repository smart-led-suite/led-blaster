<html>  
<head>
    <meta charset="UTF-8" /> 
    <meta name="viewport" content="width=device-width, initial-scale=1">

    <title>(beta) LED Steuerung @ Home</title>
    <link rel="icon" 
          type="image/ico" 
          href="favicon.ico">
    <link rel="stylesheet" media="screen" href="design.css">        

        <p id="default"> <br>
    Die LEDs wurden angeschaltet. In 5s erscheint wieder die Seite zum Ändern der Helligkeit.     
    Falls nicht, <a href="/index.html">hier klicken!</a> <br>
    (es kann sein das dadurch das LED Schalten verhindert wird) <br>

        
<?php

$mode = $_GET['mode'];  //fade einlesen
$speed = $_GET['speed'];  //speed einlesen
$time = $_GET['time']; 	//time einlesen

$luminance[0] = $_GET['w'];  //wert für weiss einlesen
$luminance[1] = $_GET['r'];  //wert für rot einlesen
$luminance[2] = $_GET['g'];  //wert für gruen einlesen
$luminance[3] = $_GET['b'];  //wert für blau einlesen

//alternative werte aus den input fields einlesen
$alternative[0] = $_GET['a_white'];
$alternative[1] = $_GET['a_red'];
$alternative[2] = $_GET['a_green'];
$alternative[3] = $_GET['a_blue'];

$colorName = array( //array for the names of the colors (matching the names in led-blaster)
0 => 'w',
1 => 'r',
2 => 'g',
3 => 'b',
 );

if($speed == "") 
{
  	$speed=1;     // default is factor 1
}
if($time == "") 
{
  	$time=1000;     // default is time=1000
}

$numberOfChangedBrightness = 0;

//wenn etwas in das Textfeld eingetragen wird, dann wird dieser wert der jeweiligen helligkeitFarbe zugeordnet
//das Textfeld überschreibt also die checkboxen
for ($color = 0; $color < 4; $color++) //some things we have to apply to each color
{
	if($alternative[$color] != "")  { 	//only use the input field value when its not an empty string
		$luminance[$color] = $alternative[$color];
	}
	if ($luminance[$color] != -1) 	//if color is -1 we don't have to do anything since it means 'no change in brightness'
	{
		$numberOfChangedBrightness++;		//if it isn't -1 we'll change the brightness. this variable is important for the wait=x command expected by led-blaster
		$luminance[$color] = $luminance[$color] * 10; //multiply value by 10 because led-blaster expects 0-1000 not 0-100
	}
}



if($mode==0) {								//fade to desired color/brightness
	$cmd = "echo mode=0 > /dev/led-blaster"; 			//enter mode 0
	$val =  shell_exec($cmd); 
	echo $cmd . "<br>";							//debugging info (only used at the beginning)
	$cmd = "echo wait=$numberOfChangedBrightness > /dev/led-blaster"; 			//set wait counter to $numberOfChangedBrightness (itll fade after changing four colorBrightnesses if everything was changed
	$val =  shell_exec($cmd); 
	echo $cmd . "<br>";							//debugging info (only used at the beginning)
	for ($color = 0; $color < 4; $color++) //write every color's brightness to fifo
	{
		if ($luminance[$color] != -1)
		{
			$cmd = "echo ". $colorName[$color] . "=" . $luminance[$color] . " > /dev/led-blaster"; 	//set each brightness WRGB
			$val =  shell_exec($cmd); 
		}
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
    
    </p><br> <br>
    <meta http-equiv="refresh" content="5; URL='/beta/index.html">
</head>    
</html>    
