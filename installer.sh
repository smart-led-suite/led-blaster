#!/bin/bash 
#script to prepare compiling of led-blaster
echo "downloading and installing pigpio-libary"
wget abyz.co.uk/rpi/pigpio/pigpio.zip
unzip pigpio.zip
cd PIGPIO
make
sudo make install
cd ..			#navigate to led-blaster folder
echo "creating directory /opt/led-blaster"
mkdir /opt/led-blaster
mv *.hpp *.cpp *.h /opt/led-blaster
cd /opt/led-blaster
echo ll
echo "making led-blaster"
make 
echo "add led-blaster to autostart"
mv DAEMON_INITFILE /etc/init.d/led-blaster
sudo chmod +x /etc/init.d/led-blaster
sudo update-rc.d led-blaster defaults
cd #return to home directory
echo "reboot your pi to start led-blaster or run /etc/init.d/led-blaster start"
return 0;

