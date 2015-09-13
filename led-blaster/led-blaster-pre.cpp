//============================================================================
// Name        : hello.cpp
// Author      : Sandesh
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
// ToDo	       : 
//	(done)		 - new universal initPin
//	(done)		 - introduce header file so the functions can be placed below the main part
//	(done)		 - introduce file to save current luminances
//	(done)		 - introduce simultaneous fadeAlgorithm
//			 - introduce separate files
//			 - improve simultaneous algorithm
//			 - move currentBrightness read/write to own function
//			 - introduce other fadeModes (aka exp fade)
//	(done, untested) - introduce pipes (major)
//		 	 - introduce some other modes (fade w/ overlap of different colors)
//			 - introduce music mode
//			 - introduce config file w/ led pins etc.
//			 - tidying up code changes due to implementation of FIFO (BLOCKER)
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
#include <sys/stat.h>
#include <unistd.h>
#include <linux/stat.h>

#include "modes.hpp"
#include "fadeModes.hpp"
#include "config.h"
#include "currentBrightnessFileRW.hpp"
#include "init.hpp"
#include "led-blaster-pre.hpp"


// MOVE #defines into header
#define FIFO_FILE	"/dev/led-blaster"

using namespace std;


//pins		w, r, g, b
int pins[4] = {25, 17,18,22};

int fadeAlgorithm = 0;
int realPWMrange = 0;
int PWMrange = 0;
int pin = 17; //pin used for continious fade on 1 pin
uint16_t mode = 0; // mode is now a global variable! set mode to 0 (default)

uint16_t targetBrightness[4]; //global because of readCurrentBrightness || maybe changed in the future since we won't need the cBFile in the future
uint16_t currentBrightness[4]; 




//***********************************************************************************************
//********************************************** MAIN *********************************************
//*********************************************************************************************	

int main(int argc, char* argv[]) {	
	bool mode1ThreadActive = false; //default is: no thread (obviusly)
	bool mode2ThreadActive = false; //default is: no thread (obviusly)
	//init variables to use with the interactive live input
	
  	uint16_t brightness;
  	uint16_t waitCounter = 0; //used only in live mode. 
	int fadeTimeMs = 1000; //time variable in ms; default is 1000
	int speed = 1; //speedvariable for mode1
	
	// FIFO vars
	FILE *fifo_file;
	
	char *lineptr = NULL, nl; // pointer to line currently being read
	size_t linelen; // length of line read
	int numberOfValues; // needed to check if anything was read, and how many parts were detected during sscanf
	// Commands are composed as following: "COMMAND=VALUE", e.g. b=50 for blue, brightness 50
	char cmd[] = "                   "; // command (before "=")
	int value; // brightness (after "=")
	
	int sleep = 50000; // us to sleep
	
	char dummy[] = "hallo"; //for whatever reason the last defined char array will be smashed into rubbish after while(true). so therefore a dummy as last char
	
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
	//if the kill command is send (SIGTERM) we want to terminate the gpios and close all open threads a bit faster than if ctrl+c is detected
	signal(SIGTERM, ledBlasterTerminateFast);
	
	
	
	
	// FIFO preparations
        //Create the FIFO if it does not exist 
        umask(0);
        mknod(FIFO_FILE, S_IFIFO|0666, 0);

        fifo_file = fopen(FIFO_FILE, "r");
	
	while(true) {
		// READ COMMAND + VALUE
		if ((numberOfValues = getline(&lineptr, &linelen, fifo_file)) < 0) { // if no lines read -> wait and repeat
			usleep(sleep);
			continue;
		}
		printf("[%d]%s\n", numberOfValues, lineptr);
		numberOfValues = sscanf(lineptr, "%[^=]=%d%c", &cmd, &value, &nl);
		
		printf("numberOfValues: %d, cmd: %s, value: %d\n", numberOfValues, cmd, value);
		//if we have the variable=value syntax we have 3 returned by sscanf
		//only in this case we want to do anything.
		if (numberOfValues == 3) 
		{
			// PROCESS COMMAND
		  	if (strcmp("exit", cmd)==0) //strcmp compares c strings and returns 0 if they are the same
		  	{
		  		ledBlasterTerminate(0); //terminates LEDBlaster. function needs an int as argument which is useless (but necessary for signal() syntax
				//return 0;
		  	}
		  	else  if (strcmp("mode", cmd)==0) 
		  	{
		  		mode = value;
		  	}
			else if (strcmp("time", cmd)==0)
		  	{
				fadeTimeMs = value;
			}
			else if (strcmp("speed", cmd)==0)
		  	{
				speed = value;
			}
			if (mode == 0) { //mode statement is here because we only need these cmds in mode0
			  	if (strcmp("wait", cmd)==0) 
			  	{
			  		waitCounter = value;
			  	}
			  	else if (strcmp("w", cmd)==0) 
			  	{
			  		targetBrightness[0] = value;
			  		if (waitCounter) 
			  		{
				  		waitCounter--;
				  	}	
			  	} else if (strcmp("r", cmd)==0) {
			  		targetBrightness[1] = value;
			  		if (waitCounter) {
				  			waitCounter--;
				  		}
			  	} else if (strcmp("g", cmd)==0) {
			  		targetBrightness[2] = value;
			  		if (waitCounter) {
				  			waitCounter--;
				  		}
			  	} else if (strcmp("b", cmd)==0) {
			  		targetBrightness[3] = value;
			  		if (waitCounter) {
				  			waitCounter--;
				  		}	
			  	}
			  	
		  	
		  		if (waitCounter == 0) {
		  			cout << "fading leds simultaneous..." << endl;
			  		fadeSimultaneous(fadeTimeMs);
			  		writeCurrentBrightness(); //write brightness so php part can read it :-)
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
				mode1ThreadActive = false; 	//make it possible to start mode 1 again
				mode2ThreadActive = false; 		//make it possible to start mode 1 again
			}
		  	else if (mode == 1) 
		  	{
		  		if (mode1ThreadActive == false) 
		  		{
			  		//for documentation about threads see: https://computing.llnl.gov/tutorials/pthreads/
			  		//and about the pthread_create function see:  https://computing.llnl.gov/tutorials/pthreads/man/pthread_create.txt
			  		pthread_t continiousFadeThread; //create reference variable for the thread
			  		//it is recommended to not use a address as attribute because the value may change until the thread is started (it's listed as bad example in the documentation.
			  		//but as we WANT to change it after the thread has started we're using it anyway. if there are any problems we comment MODE_LIVE_MANIPULATING and everything is as its recommended.
			  		#ifndef MODE_LIVE_MANIPULATING
			  			int a = pthread_create(&continiousFadeThread, NULL, NULL, modeContiniousFade , (void *) speed); //init the new thread, it can be adressed by continiousFadeThread, the speed variable will be sent as attribute, the main function of the new thread is modeContiniousFade
			  		#endif
			  		#ifdef MODE_LIVE_MANIPULATING
			  		int a = pthread_create(&continiousFadeThread, NULL, modeContiniousFade , (void *) &speed); //init the new thread, it can be adressed by continiousFadeRandomThread, the fadeTimeUs variable will be sent as attribute, the main function of the new thread is modeContiniousFadeRandom
			  		#endif
			  		if (a != 0)  //if it is successfull it returns 0, otherwise an error code
			  		{
			  			cout << "Something happened while starting the modeContiniousFade Thread" << endl;
			  			cout << "error no: " << a << endl;		//print the errorcode if one occurs
			  		} 
			  		else 							//else, the thread was successfully created
			  		{
			  		mode1ThreadActive = true; //set variable to true because thread has been created successfully
			  		cout << "start continuos fade on all pins (wrgb successively) exit with mode = 0 or Ctrl+C, DO NOT CHANGE ANY OTHER VALUE OR YOU HAVE TO REBOOT YOUR PI" << endl;
			  		}
		  		}
		  					
			}
			else if (mode == 2) 
		  	{
		  		if (mode2ThreadActive == false) 
		  		{
			  		//for documentation about threads see: https://computing.llnl.gov/tutorials/pthreads/
			  		//and about the pthread_create function see:  https://computing.llnl.gov/tutorials/pthreads/man/pthread_create.txt
			  		pthread_t continiousFadeRandomThread; //create reference variable for the thread
			  		//it is recommended to not use a address as attribute because the value may change until the thread is started (it's listed as bad example in the documentation.
			  		//but as we WANT to change it after the thread has started we're using it anyway. if there are any problems we comment MODE_LIVE_MANIPULATING and everything is as its recommended.
			  		#ifndef MODE_LIVE_MANIPULATING
			  			int a = pthread_create(&continiousFadeRandomThread, NULL, modeContiniousFadeRandom , (void *) fadeTimeMs); //init the new thread, it can be adressed by continiousFadeRandomThread, the fadeTimeUs variable will be sent as attribute, the main function of the new thread is modeContiniousFadeRandom 
			  		#endif
			  		#ifdef MODE_LIVE_MANIPULATING
			  		int a = pthread_create(&continiousFadeRandomThread, NULL, modeContiniousFadeRandom , (void *) &fadeTimeMs); //init the new thread, it can be adressed by continiousFadeRandomThread, the fadeTimeUs variable will be sent as attribute, the main function of the new thread is modeContiniousFadeRandom 
			  		#endif
			  		if (a != 0)  //if it is successfull it returns 0, otherwise an error code
			  		{
			  			cout << "Something happened while starting the modeContiniousFadeRandom Thread" << endl;
			  			cout << "error no: " << a << endl;		//print the errorcode if one occurs
			  		} 
			  		else 							//else, the thread was successfully created
			  		{
			  		mode2ThreadActive = true; //set variable to true because thread has been created successfully
			  		cout << "start continuos, random fade on all pins (wrgb successively) exit with mode = 0 or Ctrl+C, DO NOT CHANGE ANY OTHER VALUE OR YOU HAVE TO REBOOT YOUR PI" << endl;
			  		}
		  		}
		  					
			}
			gpioSleep(PI_TIME_RELATIVE, 0, 100000); //sleeps for 0.1s
			//gpioDelay(10000); //10ms some delay so it won't use that much cpu power 
		}
			
	} // END NOW-INVALID CODE 
	pthread_exit(NULL);	
}


//***************************************FUNCTIONS***************************************

//function which is called if ctrl+c is pressed in order to close led-blaster normally	
void ledBlasterTerminate(int dummy)
{
	mode = 0; //so we won't have any problems with threads and so on
  	printf("\nUser pressed Ctrl+C || SIGINT detected. Turn LEDs off.\n");
	// we want to turn all GPIOs of to avoid some strange stuff.
  	turnLedsOff(SIGINT_PIBLASTER_TERMINATE_TIME_VALUE); //turn all leds off in 1000ms = 1s so it won't take too long
	writeCurrentBrightness(); //useless but we'll save it anyway
	printf("terminate gpio \n");
	gpioTerminate(); //terminates GPIO (but doesn't necessarily turn all gpios off
	printf("close all threads \n");
	pthread_exit(NULL);
	printf("exit program. thank you. \n");
  	exit(1);
}
//function which is called when a SIGTERM is sended, i.e. by the kill command. it will close led-blaster FAST
void ledBlasterTerminateFast(int dummy)
{
	mode = 0; //so we won't have any problems with threads and so on
  	printf("\nSIGTERM detected. Turn LEDs off.\n");
	// we want to turn all GPIOs of to avoid some strange stuff.
  	turnLedsOff(SIGTERM_PIBLASTER_TERMINATE_FAST_TIME_VALUE); //turn all leds off in 50ms its fast. ;-). if that's not enough we may decrease it to 0
	writeCurrentBrightness(); //useless but we'll save it anyway
	printf("terminate gpio \n");
	gpioTerminate(); //terminates GPIO (but doesn't necessarily turn all gpios off
	printf("close all threads \n");
	pthread_exit(NULL);
	printf("exit program. thank you. \n");
  	exit(1);
}

