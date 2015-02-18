<html>
<head>
<meta name="viewport" content="width=device-width" />
<title>LED Steuerung @ Home</title>
</head>
<body>
Licht an und ausschalten:
<form action="" method="get">
  <input type="radio" value="1" name="w">Licht ein  <br>
  <input type="radio" value="0" name="w">Licht aus <br> <br> 
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
//echo "hello world1";
$fade=$_GET['fade'];
$white=$_GET['w'];
//$luminance=$_GET['luminance'];

//echo "Hello World   fade:";
//echo $fade;

if($fade == "") {
  $fade=1;  }   // default is on

//if ($white==1) {   //licht an
//   $luminance=100;
//}//
//$luminance=0;
//echo $luminance;

//$alternativeLuminance=$_GET['luminance'];
//if($alternativeLuminance!="")  {
	$luminance=$_GET['luminance'];
//}
//echo $luminance;
if ($white==1) {
	$luminance=100;
} 
//echo $luminance;

if ($white==0) {   //licht aus
  $luminance=0;
} 
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

//else if (isset($_GET['w_off'])) {
//echo "Hallo 2";
/*$cmd = "./universal.py 1 1 0 0 0 ";
echo $cmd;
$val = shell_exec($cmd);
echo $val;
echo "Licht ist aus";
}// else if (isset($_GET('submit_l'])) {
$cmd = "./universal.py $fade $luminance $luminance $luminance";
shell_exec($cmd);
//} */
?>
</body>
</html>
