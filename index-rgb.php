<!DOCTYPE HTML>
<html>
<head>
<meta charset="UTF-8">
<title>LED-Steuerung</title>
</head>
<body>
<form action="" method="GET">
	<table>
	<tr>
		<td><input type="radio" value="100" name="r">Rot an
		<td><input type="radio" value="100" name="g"/>Grün an
		<td><input type="radio" value="100" name="b"/>Blau an
	</tr>
	<tr>
		<td><input type="radio" value="0" name="r" checked>Rot aus
		<td><input type="radio" value="0" name="g" checked>Grün aus
		<td><input type="radio" value="0" name="b" checked>Blau aus
	</tr>
</table>
	<br>
	<input type="checkbox" value="1" name="fade" checked>Fade
	<input type="submit" value="LEDs schalten">
</form>

<?php
	$r = $_GET["r"];
	$g = $_GET["g"];
	$b = $_GET["b"];
	$fade = $_GET["fade"];

	$cmd = "./universal.py $fade $r $g $b";
	shell_exec($cmd);
?>

</body>
</html
