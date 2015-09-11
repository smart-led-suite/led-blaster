#! /bin/bash
#see: http://www.forum-raspberrypi.de/Thread-projekt-idee-wecker-mit-aufwecklicht
aktuellezeit=$(date +%H:%M) # Zeit lesen
weckzeit=$(cat /var/www/beta/time.txt) # Weckzeit lesen

while true
do
        if [ $aktuellezeit == $weckzeit ]
        then
                sleep 60 # Verhindert, dass die Bedingung im nächsten Durchgang zutrifft
                /var/www/beta/alarmClock.sh # Weckskript starten
                aktuellezeit=$(date +%H:%M) # Zeit lesen
                sleep 1 #Prozessor schonen
                weckzeit=$(cat /var/www/beta/time.txt) #Weckzeit lesen
        else
                sleep 28 # 28 Sekunden Pause bis  zur nächsten rüfung
                aktuellezeit=$(date +%H:%M) # Zeit lesen
                sleep 1 # Prozessor schonen
                weckzeit=$(cat /var/www/beta/time.txt) #Weckzeit lesen
        fi
done
