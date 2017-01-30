# led-blaster

project inspired by pi-blaster and based on pigpio libary (http://abyz.co.uk/rpi/pigpio/) which uses a mix between hw and sw pwm.

the goal is to create a universal code in which you write settings and so on in FIFO style (just like pi-blaster).

#Commands

echo them into /dev/led-blaster  
e.g. `echo f25:1000 > /dev/led-blaster`

Opposed to previous versions, led-blaster is now intended to behave like a stupid slave for our new node.js application (in development)
thats why we removed the colorCode and are using just the Pin instead. led-blaster is only responsible for fading, which color etc. doesn't count.

##Protocol basics

the protocol consists of the following:

1. a char defining it's meaning (e.g. f -> fade leds)
2. a number defining the LED Pin (e.g. 25) followed by a colon
3. a number defining the brightness followed by a semi-colon

2.&3. are repeated for either
- the number of LED objects available
- 100 times if it is a initialize command
this should be enough

##Known command 'meanings'
1. set brightnesses:
   the char is 's'.
   this sets the LEDs directly to the desired value (no fade)  
2. fade brightnesses:
   the char is 'f'.
   this fades the LEDs to the desired value
3. initialize:
   the char is 'i'.
   this creates LED objects and fades them to the set brightness.
   
commands for defining fadetime etc. are yet to come.
   
##example

if you have pins 18,19 and 20 defined and want to set all of them to brightness 1000 you have to write 
`s18:1000;19:1000;20:1000;` to `/dev/led-blaster` 
   
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
#THIS WILL BE REPLACED BY LED_JSCLIENT SOON!!! be ready to upgrade
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
`ps -C led-blaster` if led-blaster ist actually running.
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
