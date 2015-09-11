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


</head>
<body>
    
    <form action="" method="get">
        <p id="default">
            Weckzeit eingeben HH : MM 
            <br><br>
            <input type="time" name="alarmTime" required="required"> 
            <br><br>
            <input type="submit" value="Wecker stellen">
        </p>
    </form>
    
    <p id="default">
    <b>Weitere Funktionen: </b> <br><br>      
        <a href="weckerConfig.html">Wecker konfigurieren</a> <br>
        <a href="index.html">Home</a> <br>
    </p>

<?php
//add all commands to crontab
$alarmTime = $_GET['alarmTime'];

$timeFile = fopen('/var/www/beta/time.txt', w);
// Schreibe Stunde
fwrite ( $timeFile, $alarmTime );

fclose($timeFile);

if ($alarmTime != "") //print success message if everything was ok
{
    echo '<p id="default"> Wecker auf ' . $alarmTime . ' gestellt. <br> </p>';
}
    /*
$cronCmd = 'command="/var/www/beta/alarmClock.sh" 
job="' . $mins . ' ' . $hour . ' * * 0 $command "
cat <(fgrep -i -v "$command" <(crontab -l)) <(echo "$job") | crontab -';
$cronJobMaker = fopen('/var/www/beta/cronJobMaker.sh', w);
fwrite($cronJobMaker, "#!/bin/bash \n"); //write shebang
fwrite($cronJobMaker, $cronCmd);
fclose($cronJobMaker);
*/
/*shell_exec('var/www/beta/cronJobMaker.sh');
echo 'error: ' . $error . '<br>';
echo $cmd . '<br>';
echo $croncmd . '<br>';
echo $hour . '<br>';
echo $mins . '<br>';*/

//exec('echo  | crontab -');


//"'.$mins.' '.$hour.' * * * echo mode=0 > /dev/led-blaster; echo time=' . $fadeTime . ' /dev/led-blaster; ; ; ; echo b=100 > /dev/led-blaster;

?>
</body>
</html>
