<html>
<head>
<meta name="viewport" content="width=device-width" />
<title>LED Steuerung @ Home</title>
</head>
<body>
Licht an und ausschalten:
<form method="get" action="index-white.php">
<input type="submit" value="Licht ein" name="Lichtein">
<input type="submit" value="Licht aus" name="Lichtaus">
</form>
<form action="slot_act.php" method="post">
Time: <input name="dtime" size="7" maxlenght="4"> <!-- Hier sollen nur Zahlen rein -->

immer erst ausschalten, wenn 'Licht ist an' dasteht und umgekehrt! <br>
nach druecken des Knopfes WARTEN bis die Aktion abgeschlossen ist und das Ergebnis 'Licht ist aus/an' angezeigt wird!
<br> <br>
<?php
if(isset($_GET['Lichtein'])) {
$val = trim(@shell_exec("/usr/src/led-source/LED-Ansteuerung/on.py")); 
echo "Licht ist an";
}
else if (isset($_GET['Lichtaus'])) {
$val = trim(@shell_exec("/usr/src/led-source/LED-Ansteuerung/off.py"));
echo "Licht ist aus";
}
?>
</body>
</html>
