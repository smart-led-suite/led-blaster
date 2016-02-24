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
#include <map>
#include <vector>

#include <string>
#include <pthread.h>
#include <stdint.h> //libary which includes uint8_t etc.
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/stat.h>
#include <unistd.h>
#include <linux/stat.h>

#include "led.hpp"
#include "modes.hpp"
#include "fade.hpp"
#include "config.h"
#include "file.hpp"
#include "init.hpp"
#include "led-blaster-pre.hpp"
#include "fifo.hpp"


// MOVE #defines into header
//#define FIFO_FILE	"/dev/led-blaster"

using namespace std;


//pins		w, r, g, b
//int pins[4] = {25, 17,18,22};


int realPWMrange = 0;
int PWMrange = 0;
uint16_t mode = 0; // mode is now a global variable! set mode to 0 (default)

std::string serverPath = "/var/www/html/";
int fadeTimeMs; //time variable in ms; default is 1000

//init vector which will hold the led-object information
std::vector<LED> leds;



//***********************************************************************************************
//********************************************** MAIN *********************************************
//*********************************************************************************************

int main(int argc, char* argv[]) {
	bool mode1ThreadActive = false; //default is: no thread (obviusly)
	int threadErrorNumber = 0; //variable for pthread_create, if its 0 there was no problem
	//init variables to use with the interactive live input

  	uint16_t brightness;
  	uint16_t waitCounter = 0; //used only in live mode.



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

	readConfig();
	//init pwm
	//initializes the pigpio libary. returns 0 if there was no problem
	if(initGeneral()) {
		//if there's a problem print it out and exit the program
		cout << "initGeneral failed!" << endl;
		return 1;
	}

	//now read the colors.csv and create led.objects based on that
	readColorConfig();
	#ifdef DEBUG
		//test if it worked
		//print the leds vector
	  for (size_t ledsAvailable = 0; ledsAvailable < leds.size(); ledsAvailable++) {
	    cout << leds[ledsAvailable].getColorCode() << " ";
	    cout << leds[ledsAvailable].getPin() << " ";
	    cout << leds[ledsAvailable].getIsColor() << " ";
	    cout << leds[ledsAvailable].getCurrentBrightness() << " ";
	    cout << leds[ledsAvailable].getTargetBrightness() << endl;
	  }
	#endif
	//read brightness and fade to it
	readTargetBrightness();
	fadeSimultaneous(fadeTimeMs);

	//if ctrl+c is pressed we want to terminate the gpios and close all open threads.
	//therefore we'll want to catch the ctrl+c by the user
	//signal(SIGINT, terminateLedBlaster);
	signal(SIGINT, ledBlasterTerminate);
	//if the kill command is send (SIGTERM) we want to terminate the gpios and close all
	//open threads a bit faster than if ctrl+c is detected
	signal(SIGTERM, ledBlasterTerminateFast);




	// FIFO preparations
  //Create the FIFO if it does not exist
  umask(0);
  mknod(FIFO_FILE, S_IFIFO|0666, 0);

  fifo_file = fopen(FIFO_FILE, "r");

	cout << "led-blaster has successfully started." << endl;
	int oldModeState = mode;
	while(true) {
		readFifo(waitCounter);
		if (waitCounter == 0 && mode == 0) {
		  cout << "fading leds simultaneous..." << endl;
			//write to file before fading
			writeCurrentBrightness();
			fadeSimultaneous(fadeTimeMs);
			//fadeDirectly(); //for testing purposes
			//write brightness so php part can read it :-)
			//writeCurrentBrightness();
			cout << "fading leds simultaneous finished" << endl;

			//print some debug info of the variables
			for (size_t ledsAvailable = 0; ledsAvailable < leds.size(); ledsAvailable++) {
				cout << leds[ledsAvailable].getColorCode() << " ";
				    //cout << leds[ledsAvailable].getPin() << " ";
				    //cout << leds[ledsAvailable].getIsColor() << " ";
				    //cout << leds[ledsAvailable].getCurrentBrightness() << " ";
				cout << leds[ledsAvailable].getTargetBrightness() << endl;
			}
			cout << "waitCounter: " << waitCounter << endl;
			cout << "mode: " << mode << endl;
			cout << "waitCounter: " << waitCounter << endl;

			mode1ThreadActive = false; 	//make it possible to start mode 1 again
		}
		else if (mode == 1 && oldModeState == 0)
		  	{
		  		if (mode1ThreadActive == false)
		  		{
			  		//for documentation about threads see:
			  		//https://computing.llnl.gov/tutorials/pthreads/
			  		//and about the pthread_create function see:
			  		//https://computing.llnl.gov/tutorials/pthreads/man/pthread_create.txt
			  		pthread_t mode1Thread; //create reference variable for the thread
			  		//it is recommended to not use a address as attribute because the value
			  		//may change until the thread is started (it's listed as bad example in the documentation.
			  		//but as we WANT to change it after the thread has started we're using it anyway.
			  		//if there are any problems we comment MODE_LIVE_MANIPULATING and everything is as its recommended.
			  		#ifndef MODE_LIVE_MANIPULATING
			  			//init the new thread, it can be adressed by mode1Thread, the fadeTimeUs variable will
			  			//be sent as attribute, the main function of the new thread is mode1
			  			threadErrorNumber = pthread_create(&mode1Thread, NULL, mode1 , (void *) fadeTimeMs);
			  		#endif
			  		#ifdef MODE_LIVE_MANIPULATING
			  			//init the new thread, it can be adressed by mode1Thread
			  			//the fadeTimeUs variable will be sent as attribute
			  			//the main function of the new thread is mode1
			  			threadErrorNumber = pthread_create(&mode1Thread, NULL, mode1 , (void *) &fadeTimeMs);
			  		#endif
			  		if (threadErrorNumber != 0)  //if it is successfull it returns 0, otherwise an error code
			  		{
			  			cout << "Something happened while starting the mode1 Thread" << endl;
			  			cout << "error no: " << threadErrorNumber << endl;		//print the errorcode if one occurs
			  			threadErrorNumber = 0; //reset error number for future restarts
			  		}
			  		else 							//else, the thread was successfully created
			  		{
			  		mode1ThreadActive = true; //set variable to true because thread has been created successfully
						oldModeState = 1;
			  		cout << "start continuos, random fade on all pins (wrgb successively) exit with mode = 0 or Ctrl+C, DO NOT CHANGE ANY OTHER VALUE OR YOU HAVE TO REBOOT YOUR PI" << endl;
			  		}
		  		}

			}
			//gpioSleep(PI_TIME_RELATIVE, 0, 100000); //sleeps for 0.1s
			//gpioDelay(10000); //10ms some delay so it won't use that much cpu power
		} //INVALID CODE

}


//***************************************FUNCTIONS***************************************

//function which is called if ctrl+c is pressed in order to close led-blaster normally
void ledBlasterTerminate(int dummy)
{
	mode = 0; //so we won't have any problems with threads and so on
  	printf("\nUser pressed Ctrl+C || SIGINT detected. Turn LEDs off.\n");
	// we want to turn all GPIOs of to avoid some strange stuff.
  	turnLedsOff(SIGINT_PIBLASTER_TERMINATE_TIME_VALUE); //turn all leds off in 1000ms = 1s so it won't take too long
	//writeCurrentBrightness(); //useless but we'll save it anyway
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
	//writeCurrentBrightness(); //useless but we'll save it anyway
	printf("terminate gpio \n");
	gpioTerminate(); //terminates GPIO (but doesn't necessarily turn all gpios off
	printf("close all threads \n");
	pthread_exit(NULL);
	printf("exit program. thank you. \n");
  	exit(1);
}
