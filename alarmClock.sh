#!/bin/bash 
#script called by cronjob for waking up/alarm clock
echo mode=0 > /dev/led-blaster 
echo time=3555 > /dev/led-blaster 
echo wait=4 > /dev/led-blaster 
echo w=850 > /dev/led-blaster 
echo r=490 > /dev/led-blaster 
echo g=520 > /dev/led-blaster 
echo b=1000 > /dev/led-blaster 
