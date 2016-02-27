# led-blaster

project inspired by pi-blaster and based on pigpio libary (http://abyz.co.uk/rpi/pigpio/) which uses a mix between hw and sw pwm.

the goal is to create a universal code in which you write settings and so on in FIFO style (just like pi-blaster).
the following things are on our toDo-list:

1. assign pins to led colors (i.e. 17 = blue led)
2. enter desired fade mode (linear, exp., no fade)
3. enter target brigthness for each led/for all rgbw leds (the program will then fade according to the fade mode
4. option to connect a sound sensor
5. some additional modes like continious fade to random color w/ defined speed etc.
6. an alarm clock which fades on all/some leds slowly at a given time (sunrise simulation to wake up easily) (in coordination with some PHP code...)
7. more to come...

#Commands

echo them into /dev/led-blaster  
e.g. `echo w=1000 > /dev/led-blaster`

1. RGBW brightnesses:

   r = value  //set redBrightness to value 0...RANGE  
   g = value  //set greenBrightness to value 0...RANGE  
   b = value  //set blueBrightness to value 0...RANGE  
   w = value  //set whiteBrightness to value 0...RANGE  

2. modes:
   mode = value //choose mode obviously

   options:  
   mode0 = fade to fixed brightness  
   mode1 = fade to random colors  

3. time:
  time = value
  select the time (in ms) needed until fade is completed

#Installation of a Release

1. create a new folder: `sudo mkdir /opt/led-blaster`
2. move the downloaded binaries to this folder `mv led-blaster-pre /opt/led-blaster`
3. now find the DAEMON_INITFILE from the source code .zip and do the following  
```
sudo mv DAEMON_INITFILE /etc/init.d/led-blaster #move initfile
sudo chmod +x /etc/init.d/led-blaster #make it executable
sudo update-rc.d led-blaster defaults #update
```
You have successfully added led-blaster to your autostart!

#Compiling led-blaster

##Install pipio libary
led-blaster needs the [pigpio libary](http://abyz.co.uk/rpi/pigpio/)
to install it, go to your home directory with `cd ` (or wherever you want to install it) and run:
```
wget abyz.co.uk/rpi/pigpio/pigpio.zip
unzip pigpio.zip
cd PIGPIO
make
sudo make install
```
##Install webserver (apache) and led-smarthome
led-blaster assumes you have a webeserver running and the led-smarthome php files stored there.
e.g. the information which pins are actually used is stored there.
so please install the webserver and [led-smarthome](https://github.com/smart-led-suite/led-smarthome) first.
##Install led-blaster
we recommend to install led-blaster at /opt/led-blaster.
therefore you have to go to /opt via
```
cd /opt
```
then clone this repository (run with sudo if necessary)
```
git clone https://github.com/smart-led-suite/led-blaster
```
now into the led-blaster folder and compile the program:
```
cd led-blaster/
make
```
now the program is ready.
in order to run it everytime your pi boots,just do the following:
```
sudo mv DAEMON_INITFILE /etc/init.d/led-blaster #move initfile
sudo chmod +x /etc/init.d/led-blaster #make it executable
sudo update-rc.d led-blaster defaults #update
```
##Check if it is working
now reboot your pi and check with
`ps -C led-blaster-pre` if led-blaster ist actually running.
##Deinstallation
if you delete `/etc/init.d/led-blaster` led-blaster is removed from autostart. you can then remove all the files and uninstall the pigpio_libary
#Configuration
you can configure led-blaster in various ways:

1. the global config file:  
  * open the config file at /etc/led-blaster.ledconfig (e.g. `sudo nano /etc/led-blaster.ledconfig`) 
  * you have to set the serverPath to the directory where you've installed [led-smarthome](https://github.com/smart-led-suite/led-smarthome)
  * edit the values then restart led-blaster / reboot your pi.  
2. configuration via led-smarthome
  * visit [your_pis_ip_adress]/configuration.php  
  * change the pin and the colorinformation etc. of the leds.  
  * **NOTE:** changes to the pins only apply AFTER REBOOT.  
