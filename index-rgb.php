
<!DOCTYPE html>
<html>
<meta charset="UTF-8">

<body>

<h1>Beleuchtungssteuerung</h1>

<?php
	echo "Hello World";
?>
<form action="index.php" method="GET">
<table>
	<tr>
		<td><input type="radio" value="an" name="r">Rot an
		<td><input type="radio" value="an" name="g"/>Grün an
		<td><input type="radio" value="an" name="b"/>Blau an
	</tr>
	<tr>
		<td><input type="radio" value="aus" name="r">Rot aus
		<td><input type="radio" value="aus" name="g">Grün aus
		<td><input type="radio" value="aus" name="b">Blau aus
	</tr>
</table>
<br>
<input type=submit value="Lichter schalten"/>
</form>
<?php
$pin_red = 17;
$pin_green = 18;
$pin_blue = 22;
function switch_leds($pin, $value) {
        $cmd = "echo \"$pin=$value\" > /dev/pi-blaster";
	echo shell_exec($cmd);
}
$red = $_GET["r"];
if($red != ""){
	echo "Rot: $red<br>";
	if ($red == "an") {
        	switch_leds($pin_red, 1);
	} elseif ($red == "aus") {
        	switch_leds($pin_red, 0);
	}
} else {
	echo "Rot: Zustand nicht bekannt<br>";
}
$green = $_GET["g"];
if($green != "") {
	echo "Grün: $green<br>";
	if ($green == "an") {
		switch_leds($pin_green, 1);
	} elseif ($green == "aus") {
		switch_leds($pin_green, 0);
	}
} else {
	echo "Grün: Zustand nicht bekannt<br>";
}
$blue = $_GET["b"];
if($blue != "") {
	echo "Blau: $blue<br>";
	if ($blue == "an") {
		switch_leds($pin_blue, 1);
	} elseif ($blue == "aus") {
		switch_leds($pin_blue, 0);
	}
} else {
	echo "Blau: Zustand nicht bekannt<br>";
}	
?>
<br>
</body>
</html>
