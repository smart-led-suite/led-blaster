//============================================================================
// Name        : hello.cpp
// Author      : Sandesh
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
// ToDo	       : 
//	(done)	 - new universal initPin
//	(done)	 - introduce header file so the functions can be placed below the main part
//	(done)	 - introduce file to save current luminances
//	(done)	 - introduce simultaneous fadeAlgorithm
//		 - introduce separate files
//		 - improve simultaneous algorithm
//		 - move currentBrightness read/write to own function
//		 - introduce other fadeModes (aka exp fade)
//		 - introduce pipes (major)
//		 - introduce some other modes (fade w/ overlap of different colors)
//		 - introduce music mode
//		 - introduce config file w/ led pins etc. 
//============================================================================

#include <iostream>
#include <cstdlib>
#include <pigpio.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <pthread.h>
#include <stdint.h> //libary which includes uint8_t etc.
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#include "modes.hpp"
#include "fadeModes.hpp"
#include "config.h"
#include "currentBrightnessFileRW.hpp"
#include "init.hpp"
#include "led-blaster-pre.hpp"

using namespace std;


//pins		w, r, g, b
int pins[4] = {25, 17,18,22};

int fadeAlgorithm = 0;
int realPWMrange = 0;
int PWMrange = 0;
int fadeDelayUs = FADE_DELAY_US;
int pin = 17; //pin used for continious fade on 1 pin
uint16_t mode = 0; // mode is now a global variable! set mode to 0 (default)
bool modeOnethreadWasCreated = false; //default is: no thread (obviusly)
uint16_t targetBrightness[4]; //global because of readCurrentBrightness || maybe changed in the future since we won't need the cBFile in the future
uint16_t currentBrightness[4]; 




//***********************************************************************************************
//********************************************** MAIN *********************************************
//*********************************************************************************************	

int main(int argc, char* argv[]) {		
	
	//init variables to use with the interactive live input
	char variable[5] = "    ";
	char dummy[] = "hallo"; //for whatever reason the last defined char array will be smashed into rubbish after while(true). so therefore a dummy as last char
  	uint16_t value = 0;
  	uint16_t brightness;
  	uint16_t waitCounter = 0; //used only in live mode. 
  	int scanSuccess = 0;
  	//init variables needed for thread creation
  	
	
	//OPEN CONFIG FILE IN OUR APPLICAITONS DIRECTORY OR CREATE IT IF IT DOESN'T EXIST
	readCurrentBrightness();
	
	//init pwm
	if(initGeneral()) {				//initializes the pigpio libary. returns 0 if there was no problem
		cout << "initGeneral failed!" << endl;	//if there's a problem print it out and exit the program
		return 1;
	}
	
	//initialize all pins
	for (int color = 0; color < COLORS; color++) {
		if(initPin(pins[color], mode) != 0) {  				//initializes each pin. returns 0 if everything went ok
			cout << "error in initPin " << pins[color] << endl;	//print that there has been an error if this happens (very unlikely)
			return 2;						//and exit the program
		}	
	}
	
	
  	
  	//some kind of manual at the beginning
	printf ("Welcome to your LED fade program \n");
	printf ("to exit, enter exit = 1 or press Ctrl+C \n");
	printf ("enter redb / grnb / blub / whtb = value to set brightness for a specific color (range 0 t 1000 \n");
	printf ("Enter mode = 0/1 to set mode; 0 = set brightness; 1 = continious fade \n");
	printf ("Enter wait = x; x=0: fade to brightness; x>0: wait x times until updating brightness again \n");
	
	//if ctrl+c is pressed we want to terminate the gpios and close all open threads. therefore we'll want to catch the ctrl+c by the user
	//signal(SIGINT, terminateLedBlaster);
	signal(SIGINT, ledBlasterTerminate);
	
	while(true) {
	
	  	cout << "Enter your configuration:  ";
	  	scanSuccess = scanf ("%s = %d",&variable, &value);  
	  	printf("variable changed: %s set to %d. \n", variable, value);
	  	printf("return value (number of scanned variables): %d \n", scanSuccess);
	  	if (strcmp("exit", variable)==0) //strcmp compares c strings and returns 0 if they are the same
	  	{
	  		ledBlasterTerminate(0); //terminates LEDBlaster. function needs an int as argument which is useless (but necessary for signal() syntax
			//return 0;
	  	}
	  	else  if (strcmp("mode", variable)==0) 
	  	{
	  		mode = value;
	  	}
	  	if (mode == 0) { //mode statement is here because we only need these variables in mode0
		  	if (strcmp("wait", variable)==0) 
		  	{
		  		waitCounter = value;
		  	}
		  	else if (strcmp("whtb", variable)==0) 
		  	{
		  		targetBrightness[0] = value;
		  		if (waitCounter) 
		  		{
			  		waitCounter--;
			  	}	
		  	} else if (strcmp("redb", variable)==0) {
		  		targetBrightness[1] = value;
		  		if (waitCounter) {
			  			waitCounter--;
			  		}
		  	} else if (strcmp("grnb", variable)==0) {
		  		targetBrightness[2] = value;
		  		if (waitCounter) {
			  			waitCounter--;
			  		}
		  	} else if (strcmp("blub", variable)==0) {
		  		targetBrightness[3] = value;
		  		if (waitCounter) {
			  			waitCounter--;
			  		}	
		  	}
		  	
	  	
	  		if (waitCounter == 0) {
	  			cout << "fading leds simultaneous..." << endl;
		  		fadeSimultaneous(fadeDelayUs, targetBrightness);
		  		//fadeDirectly(targetBrightness); //for testing purposes
		  		cout << "fading leds simultaneous finished" << endl;
		  	}
		  	cout << "waitCounter: " << waitCounter << endl;
			cout << "mode: " << mode << endl;
			cout << "waitCounter: " << waitCounter << endl;
			cout << "targetBrightness w " << targetBrightness[0] << endl;
			cout << "targetBrightness r " <<  targetBrightness[1] << endl;
			cout << "targetBrightness g " <<  targetBrightness[2] << endl;
			cout << "targetBrightness b " <<  targetBrightness[3] << endl;
		}
	  	else if (mode==1) 
	  	{
	  		if (modeOnethreadWasCreated == false) 
	  		{
		  		pthread_t continiousFadeThread; //create reference variable for the thread
		  		int a = pthread_create(&continiousFadeThread, NULL, modeContiniousFade , NULL); //init the new thread, it can be adressed by continiousFadeThread, there are no attributes sent, the main function of the new thread is modeContiniousFade
		  		if (a) 
		  		{
		  			cout << "Something happened while starting the modeContiniousFade Thread" << endl;
		  			cout << "error no: " << a << endl;
		  		}
		  		modeOnethreadWasCreated == true;
	  		}
	  					
		}
		
		gpioSleep(PI_TIME_RELATIVE, 0, 100000); //sleeps for 0.1s
		//gpioDelay(10000); //10ms some delay so it won't use that much cpu power 
			
	}
	pthread_exit(NULL);	
}


//***************************************FUNCTIONS***************************************

	
void ledBlasterTerminate(int dummy)
{
	mode = 0; //so we won't have any problems with threads and so on
  	printf("\nUser pressed Ctrl+C. Turn LEDs off.\n");
	// we want to turn all GPIOs of to avoid some strange stuff.
  	for (int i=0; i < 3; i++) {  
		targetBrightness[i] = 0;	
	}
	fadeSimultaneous(fadeDelayUs, targetBrightness); //fade LEDs OFF
	writeCurrentBrightness(); //useless but we'll save it anyway
	printf("terminate gpio \n");
	gpioTerminate(); //terminates GPIO (but doesn't necessarily turn all gpios off
	printf("close all threads \n");
	pthread_exit(NULL);
	printf("exit program. thank you. \n");
  	exit(1);
}

