//MODES
#include "modes.hpp"
#include "fadeModes.hpp"
#include "config.h"
#include "currentBrightnessFileRW.hpp"
#include "init.hpp"
#include "led-blaster-pre.hpp"

#include <iostream>
#include <cstdlib>
#include <pigpio.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>       /* for the random function :time */
#include <signal.h>
#include <string.h>
#include <string>
#include <pthread.h>
#include <stdint.h> //libary which includes uint8_t etc.
using namespace std;


//continious fade (mode = 1)
// this function is initialized w/ a pointer in order to create a separate thread in main()
void *modeContiniousFade(void* speed) 
{
	int speedFactor = 0;
	//it is recommended to not use a address as attribute because the value may change until the thread is started (it's listed as bad example in the documentation.
	//but as we WANT to change it after the thread has started we're using it anyway. if there are any problems we comment MODE_LIVE_MANIPULATING and everything is as its recommended.
	#ifndef MODE_LIVE_MANIPULATING
		if ((int)speed < 1) //if the speedfactor is 0 (i.e. no fade, just flickering) we'll set it to 1 (default delay)
		{
			speedFactor = 1;
		} 
		else
		{
			speedFactor = (int)speed; // turn speed pointer in a int and save it as speedFactor
		}	
	#endif
	turnLedsOff(1000);
	
	//if ctrl+c is pressed we want to terminate the gpios and close all open threads. therefore we'll want to catch the ctrl+c by the user
	//signal(SIGINT, terminateLedBlaster);
	signal(SIGINT, ledBlasterTerminate);
		//if the kill command is send (SIGTERM) we want to terminate the gpios and close all open threads a bit faster than if ctrl+c is detected
	signal(SIGTERM, ledBlasterTerminateFast);
	
	
	while(mode == 1) // so this thread will be deleted if the mode is changed
	{
		#ifdef MODE_LIVE_MANIPULATING
			if (* (int *)speed < 1) //if the speedfactor is 0 (i.e. no fade, just flickering) we'll set it to 1 (default delay)
		{
			speedFactor = 1;
		} 
		else
		{
			speedFactor = * (int*)speed; // turn speed pointer in a int and save it as speedFactor
		}		
		#endif
		for (int color = 0; color < COLORS; color++)  //color after color in wrgb order
		{
			while(currentBrightness[color] < realPWMrange)  //first we'll increase the brightness for every possible step
			{
				currentBrightness[color]++;			//increase global brightness variable
				gpioPWM(pins[color], currentBrightness[color]);	//set pin to new brightness
				gpioDelay(FADE_DELAY_US * speedFactor);		//make a delay. FADE_DELAY_US is specified in config.h; speedFactor is changed live
				//cout << currentBrightness[color] << endl;
			}
			while(currentBrightness[color] > 0) //then we'll decrease the brightness for every possible step
			{
				currentBrightness[color]--;			//decrease global brightness variable
				gpioPWM(pins[color], currentBrightness[color]);	//set pin to new brightness
				gpioDelay(FADE_DELAY_US * speedFactor);		//make a delay. FADE_DELAY_US is specified in config.h; speedFactor is changed live
				//cout << currentBrightness[color] << endl;
			}
		}	
	}
	turnLedsOff(1000); //turn all leds off so we can start from the beginning in the next mode
	mode = 0; //set mode to 0 to be sure, maybe we'll delete this in the future
	pthread_exit(NULL); //exit this thread
		
}




//continious fade w/random color (mode = 2)
// this function is initialized w/ a pointer in order to create a separate thread in main()
void *modeContiniousFadeRandom(void* fadeTimePointer) 
{
	 /* initialize random seed: */
  	srand (time(NULL));
	
	int fadeTime = 0;
	//it is recommended to not use a address as attribute because the value may change until the thread is started (it's listed as bad example in the documentation.
	//but as we WANT to change it after the thread has started we're using it anyway. if there are any problems we comment MODE_LIVE_MANIPULATING and everything is as its recommended.
	#ifndef MODE_LIVE_MANIPULATING
		if ((int)fadeTimePointer < 1) //if the speedfactor is 0 (i.e. no fade, just flickering) we'll set it to 1 (default delay)
		{
			fadeTime = 1000; //1000 as default
		} 
		else
		{
			fadeTime = (int)fadeTimePointer; // turn speed pointer in a int and save it as speedFactor
		}	
	#endif
	turnLedsOff(1000);
	
	//if ctrl+c is pressed we want to terminate the gpios and close all open threads. therefore we'll want to catch the ctrl+c by the user
	//signal(SIGINT, terminateLedBlaster);
	signal(SIGINT, ledBlasterTerminate);
		//if the kill command is send (SIGTERM) we want to terminate the gpios and close all open threads a bit faster than if ctrl+c is detected
	signal(SIGTERM, ledBlasterTerminateFast);
	
	
	while(mode == 2) // so this thread will be deleted if the mode is changed
	{
		for (int color = 1; color < COLORS; color++) //generate random brightness for every color exept white because that doesn't look nice.
		{  
		targetBrightness[color] = rand() % 1001;	 //rand % 1000 creates numbers between 0 and 999, ... % 1001 should create nubers between 0 and 1000
		}
		#ifdef MODE_LIVE_MANIPULATING
			if (* (int *)fadeTimePointer < 1) //if the speedfactor is 0 (i.e. no fade, just flickering) we'll set it to 1 (default delay)
		{
			fadeTime = 1;
		} 
		else
		{
			fadeTime = * (int*)fadeTimePointer; // turn speed pointer in a int and save it as speedFactor
		}		
		#endif
		fadeSimultaneous(fadeTime);	
	}
	turnLedsOff(1000); //turn all leds off so we can start from the beginning in the next mode
	mode = 0; //set mode to 0 to be sure, maybe we'll delete this in the future
	pthread_exit(NULL); //exit this thread
		
}

