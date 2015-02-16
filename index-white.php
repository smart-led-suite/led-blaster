<html>
<head>
<meta name="viewport" content="width=device-width" />
<title>LED Steuerung @ Home</title>
</head>
<body>
Licht an und ausschalten:
<form action="index-white.php" method="get">
  <input type="submit" value="Licht ein" name="w_on"> 
  <input type="submit" value="Licht aus" name="w_off"> <br> <br> 
  targetLuminance eingeben: 0 bis 100: 
  <input type="number" name="luminance" min="0" max="100">
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
//echo "hello world1";
$fade=$_GET['fade'];
$luminance=$_GET['luminance'];
//echo "Hello World   fade:";
//echo $fade;

if(isset($_GET['w_on'])) {
$cmd = "./universal.py 1 100 100 100";
echo $cmd;
$val =  shell_exec($cmd); 
echo "Licht ist an";
}
else if (isset($_GET['w_off'])) {
echo "Hallo 2";
$cmd = "./universal.py 1 1 0 0 0 ";
echo $cmd;
$val = shell_exec($cmd);
echo $val;
echo "Licht ist aus";
}// else if (isset($_GET('submit_l'])) {
$cmd = "./universal.py $fade $luminance $luminance $luminance";
shell_exec($cmd);
//}
?>
</body>
</html>
