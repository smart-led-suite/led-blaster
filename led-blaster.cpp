//============================================================================
// Name        : led-blaster
// Author      : sansha
// Version     : 0.9
// Description : daemon to fade leds
// ToDo	       :
//	(done)		 - new universal initPin
//	(done)		 - introduce header file so the functions can be placed below the main part
//	(done)		 - introduce file to save current luminances
//	(done)		 - introduce simultaneous fadeAlgorithm
//	(done) 		 - introduce separate files
//	(done)		 - improve simultaneous algorithm
//	(done)		 - move currentBrightness read/write to own function
//			 - introduce other fadeModes (aka exp fade)
//	(done) 		 - introduce pipes (major)
//			 - introduce music mode
//  (done)		 - introduce config file w/ led pins etc.
//	(done) 		 - tidying up code changes due to implementation of FIFO (BLOCKER)
//	(done)		 - make it object oriented
//	(done)		 - move fade into led-class
//	(done)		 - introduce fading in seperate threads
//============================================================================

#include "config.h"
#include "desktop-config.h"
#include <iostream>
#include <cstdlib>
#ifndef DESKTOP
#include <pigpio.h>
#endif
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

//#include "file.hpp"
//#include "init.hpp"
#include "led-blaster.hpp"
#include "fifo.hpp"


using namespace std;


//***********************************************************************************************
//********************************************** MAIN *********************************************
//*********************************************************************************************

int main() {
	//init pwm
	//initializes the pigpio libary. returns 0 if there was no problem
	if(LED::initGeneral()) {
		//if there's a problem print it out and exit the program
		cout << "initGeneral failed!" << endl;
		return 1;
	}
	//************ SETUP TERMINATION HANDLING ******************
	//if ctrl+c is pressed we want to terminate the gpios and close all open threads.
	//therefore we'll want to catch the ctrl+c by the user
	signal(SIGINT, ledBlasterTerminate);
	//if the kill command is send (SIGTERM) we want to terminate the gpios and close all
	//open threads a bit faster than if ctrl+c is detected
	signal(SIGTERM, ledBlasterTerminateFast);

	// FIFO preparations
  //Create the FIFO if it does not exist
  umask(0);
  mknod(FIFO_FILE, S_IFIFO|0666, 0);

  cout << "led-blaster has  started." << endl;
	//*********** LOOP *************************************

	while(true) {
		//blocking read from fifo
		readFifo();
    std::cout << "fifo read done" << '\n';
    usleep(1000 * 1000); //sleep 1s
	}
 //INVALID CODE
 ledBlasterTerminate(0);
}

//***************************************TERMINATING FUNCTIONS***************************************

//function which is called if ctrl+c is pressed in order to close led-blaster normally
void ledBlasterTerminate(int dummy)
{
	//mode = 0; //so we won't have any problems with threads and so on
  printf("\nUser pressed Ctrl+C || SIGINT detected. Turn LEDs off.\n");
	// we want to turn all GPIOs of to avoid some strange stuff.
  LED::setFadeTime(SIGINT_PIBLASTER_TERMINATE_TIME_VALUE);
  LED::fadeAllLedsOff();
  for(auto const &iterator : LED::ledMap)
  {
    delete iterator.second;
  }
	printf("terminate gpio \n");
  #ifndef DESKTOP
	 gpioTerminate(); //terminates GPIO (but doesn't necessarily turn all gpios off
  #endif
	printf("close all threads \n");
	//pthread_exit(NULL);
	printf("exit program. thank you. \n");
  exit(1);
}
//function which is called when a SIGTERM is sended, i.e. by the kill command. it will close led-blaster FAST
void ledBlasterTerminateFast(int dummy)
{
	//mode = 0; //so we won't have any problems with threads and so on
  printf("\nSIGTERM detected. Turn LEDs off.\n");
	// we want to turn all GPIOs of to avoid some strange stuff.
  LED::setFadeTime(SIGTERM_PIBLASTER_TERMINATE_FAST_TIME_VALUE);
  LED::fadeAllLedsOff();
  for(auto const &iterator : LED::ledMap)
  {
    delete iterator.second;
  }
	printf("terminate gpio \n");
  #ifndef DESKTOP
   gpioTerminate(); //terminates GPIO (but doesn't necessarily turn all gpios off
  #endif
	printf("close all threads \n");
	//pthread_exit(NULL);
	printf("exit program. thank you. \n");
	exit(1);
}