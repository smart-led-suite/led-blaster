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
bool threadWasCreated = false; //default is: no thread (obviusly)
uint16_t targetBrightness[4]; //global because of readCurrentBrightness || maybe changed in the future since we won't need the cBFile in the future
uint16_t currentBrightness[4]; 

//***********************************************************************************************
//********************************************** MAIN *********************************************
//*********************************************************************************************	

int main(int argc, char* argv[]) {	
	
	
	
	
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
	
	//init variables to use with the interactive live input
	char variable[5];
	//command names, its only possible to compare char arrays
	char mode_cmd[] = "mode";
	char wait_cmd[] = "wait";
  	char redb_cmd[] = "redb";
  	char grnb_cmd[] = "grnb";
  	char blub_cmd[] = "blub";
  	char whtb_cmd[] = "whtb";
  	char exit_cmd[] = "exit";
  	
  	
  	//values
  	uint16_t wait=0;
  	uint16_t value = 0;
  	uint16_t brightness;
  	uint16_t waitCounter = 0; //used only in live mode. 
  	uint16_t waitCounterW = 0;
  	uint16_t waitCounterR= 0;
  	uint16_t waitCounterG = 0;
  	uint16_t waitCounterB = 0;
 	// char* pointer = NULL,  nl;
  	int scanSuccess = 0;
  	
  	/*char *lineptr = NULL, nl;
  	int n, servo;
  	float value;*/
	
//	int mode = 0;
	//int targetBrightness[4];
	#ifdef CLI_FADE
	printf ("Welcome to your LED fade program \n");
	printf ("enter exit = 1 to exit. doesn't work for whatever reason :( \n");
	printf ("enter redb / grnb / blub / whtb = value to set brightness for a specific color \n");
	printf ("Enter mode = 0/1 to set mode; 0 = set brightness; 1 = continious fade \n");
	printf ("Enter wait = x; x=0: fade to brightness; x>0: wait x times until updating brightness again \n");
	#endif
	
	const char *configFilename = "inputData.txt";
	FILE *confFile;
	
	char fileInputString[5] = "    ";
	char fileInputValue[20] = "                   ";
	
	

while(true) {
	
	  	cout << "(loop start) Enter your configuration:  ";
	  	scanSuccess = scanf ("%s = %d",&variable, &value);  
	  	printf("variable changed: %s set to %d. \n", variable, value);
	  	printf("return value (number of scanned variables): %d \n", scanSuccess);
	  	if (strcmp(exit_cmd, variable)==0) {
	  		printf("exit program. thank you. \n");
			writeCurrentBrightness();
			gpioTerminate();
			return 0;
	  	} else if (strcmp(mode_cmd, variable)==0) {
	  		mode = value;
	  	} else if (strcmp(wait_cmd, variable)==0) {
	  		wait = value;
	  		waitCounter = value;
	  	} else if (strcmp(whtb_cmd, variable)==0) {
	  		targetBrightness[0] = value;
	  		if (wait) {
		  			waitCounter--;
		  		}	
	  	} else if (strcmp(redb_cmd, variable)==0) {
	  		targetBrightness[1] = value;
	  		if (wait) {
		  			waitCounter--;
		  		}
	  	} else if (strcmp(grnb_cmd, variable)==0) {
	  		targetBrightness[2] = value;
	  		if (wait) {
		  			waitCounter--;
		  		}
	  	} else if (strcmp(blub_cmd, variable)==0) {
	  		targetBrightness[3] = value;
	  		if (wait) {
		  			waitCounter--;
		  		}	
	  	}
		  	
	  	if (mode == 0) {
	  		/* if (threadWasCreated)
	  			pthread_exit(NULL); */
	  		if (wait == 0 || waitCounter == 0) {
	  			cout << "fading leds simultaneous..." << endl;
		  		fadeSimultaneous(fadeDelayUs, targetBrightness);
		  		//fadeDirectly(targetBrightness); //for testing purposes
		  		cout << "fading leds simultaneous finished" << endl;
		  	}
		  	cout << "waitCounter: " << waitCounter << endl;
			cout << "mode: " << mode << endl;
			cout << "wait: " << wait << endl;
			cout << "targetBrightness w " << targetBrightness[0] << endl;
			cout << "targetBrightness r " <<  targetBrightness[1] << endl;
			cout << "targetBrightness g " <<  targetBrightness[2] << endl;
			cout << "targetBrightness b " <<  targetBrightness[3] << endl;
		  }
	  	
	  	
	  	
	  	if (mode==1) {
	  		
	  		pthread_t continiousFadeThread;
	  		void *randomPointer;
	  		//void (*modeContFadePointer)(void);
	  		//modeContFadePointer = modeContiniousFade;
	  		int a = pthread_create(&continiousFadeThread, NULL, modeContiniousFade , NULL);
	  		if (a) {
	  			cout << "Something happened while starting the modeContiniousFade Thread" << endl;
	  			cout << "error no: " << a << endl;
	  			}
	  			
	  		threadWasCreated = true;
	  		//modeContiniousFade();
			
		}
		
		gpioSleep(PI_TIME_RELATIVE, 0, 100000); //sleeps for 0.1s
		//gpioDelay(10000); //10ms some delay so it won't use that much cpu power 
			
	}
	pthread_exit(NULL);	
}


//***************************************FUNCTIONS***************************************


