<html>
<head>
<meta charset="UTF-8" /> 
<meta name="viewport" content="width=device-width, initial-scale=1">

<title>SmartWecker</title>
<link rel="icon" 
      type="image/ico" 
      href="favicon.ico">
    
<!-- import custom stylesheet -->   
<link rel="stylesheet" media="screen" href="design.css">    

    
<?php
$fadeTime = $_GET['fadeTime'];

$luminance[0] = $_GET['w'];  //wert für weiss einlesen
$luminance[1] = $_GET['r'];  //wert für rot einlesen
$luminance[2] = $_GET['g'];  //wert für gruen einlesen
$luminance[3] = $_GET['b'];  //wert für blau einlesen




$alarmScript = fopen('/var/www/beta/alarmClock.sh', w);

fwrite($alarmScript, "#!/bin/bash \n"); //write shebang
fwrite($alarmScript, "#script called by cronjob for waking up/alarm clock\n"); //write shebang
$cmd =  "echo mode=0 > /dev/led-blaster 
echo time=" . $fadeTime . " > /dev/led-blaster 
echo wait=4 > /dev/led-blaster 
echo w=" . $luminance[0] . " > /dev/led-blaster 
echo r=" . $luminance[1] . " > /dev/led-blaster 
echo g=" . $luminance[2] . " > /dev/led-blaster 
echo b=" . $luminance[3] . " > /dev/led-blaster \n";

//$croncmd = 'crontab -l | { cat;  echo '. $mins.' '. $hour . ' * * * /var/www/beta/alarmClock.sh"; } | sudo crontab -';
//echo "'. $mins.' '. $hour . ' * * * */



fwrite($alarmScript, $cmd);
fclose($alarmScript);

?>
	<p id="default">
	<meta http-equiv="refresh" content="5; URL='/beta/wecker.php'">
	<p id="default"> <br>
	    Der Wecker wurde konfiguriert, In 5s erscheint die Seite zur Einstellung der Weckzeit.     
	    Falls nicht, <a href="wecker.php">hier klicken!</a> <br> 

	Falls die Einstellungen falsch waren und du nochmals konfigurieren willst,<a href="weckerConfig.html">hier klicken!</a> <br> 
	</p>

        

</head>
