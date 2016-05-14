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
//			 - improve simultaneous algorithm
//	(done)		 - move currentBrightness read/write to own function
//			 - introduce other fadeModes (aka exp fade)
//	(done) 		 - introduce pipes (major)
//		 	 - introduce some other modes (fade w/ overlap of different colors)
//			 - introduce music mode
//  (done)		 - introduce config file w/ led pins etc.
//	(done) 		 - tidying up code changes due to implementation of FIFO (BLOCKER)
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


using namespace std;

//std::vector<LED> leds;

/*struct ledInformation {
  std::vector<LED> leds;
  int fadeTime;

};*/
struct ledInformationStruct fadeInfo;
//***********************************************************************************************
//********************************************** MAIN *********************************************
//*********************************************************************************************

int main(int argc, char* argv[]) {
  struct configInformationStruct config;

//  int fadeTimeMs; //time variable in ms; default is 1000
  //FADEMODE 1 variables
  //uint16_t mode = 0; //set mode to 0 (default)
  //as fademode 1 runs in a seperate thread we need a few more variables here
  pthread_t mode1Thread; //create reference variable for the thread
	bool mode1ThreadActive = false; //default is: no thread (obviusly)
	int threadErrorNumber = 0; //variable for pthread_create, if its 0 there was no problem
	//init variables to use with the interactive live input
	//since we're writing the brightness not simultaneously we want to
	//wait until we call fadeSimultaneous()
//  uint16_t waitCounter = 0; //used only in live mode.
	//reading global config
	readConfig(&fadeInfo, &config);
	//init pwm
	//initializes the pigpio libary. returns 0 if there was no problem
	if(initGeneral()) {
		//if there's a problem print it out and exit the program
		cout << "initGeneral failed!" << endl;
		return 1;
	}
	//now read the colors.csv and create led.objects based on that
	if(readColorConfig( &fadeInfo))
  {
    std::cerr << "there was a problem while reading your color/pin configuration" << std::endl;
  }
  //fadeInfo.fadeTime = fadeTimeMs;

	#ifdef DEBUG
		//test if it worked
		//print the leds vector
	  for (size_t ledsAvailable = 0; ledsAvailable < leds.size(); ledsAvailable++) {
	    cout << fadeInfo.leds[ledsAvailable].getColorCode() << " ";
	    cout << fadeInfo.leds[ledsAvailable].getPin() << " ";
	    cout << fadeInfo.leds[ledsAvailable].getIsColor() << " ";
	    cout << fadeInfo.leds[ledsAvailable].getCurrentBrightness() << " ";
	    cout << fadeInfo.leds[ledsAvailable].getTargetBrightness() << endl;
	  }
	#endif
	//read brightness and fade to it
	readTargetBrightness(&fadeInfo);
  //at the first time we have to set the number of steps
  fadeInfo.pwmSteps = fadeInfo.leds[0].getPwmSteps(); //should be the same for every led so we just use the first one
  //setFadeSteps(&pwmSteps);
	fadeSimultaneous(&fadeInfo);
	//************ SETUP TERMINATION HANDLING ******************
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

	cout << "led-blaster has successfully started." << endl;

    config.mode = 0;
	//var to check if mode was changed
	int oldModeState = config.mode;
	//*********** LOOP *************************************
	while(true) {
		//blocking read from fifo
		readFifo(&config, &fadeInfo);
		//if we're in mode0 we want to fade the pins to their brightness
		if (config.waitCounter == 0 && config.mode == 0) {
      //kill mode1 if active
      if (mode1ThreadActive)
      {
        pthread_cancel(mode1Thread);
        #ifdef DEBUG
          std::cout << "mode 1 canceled" << std::endl;
        #endif
        mode1ThreadActive = false; 	//make it possible to start mode 1 again
      }

			//write to file before fading
			writeCurrentBrightness(&fadeInfo);
      cout << "fading leds simultaneous..." << endl;
			fadeSimultaneous(&fadeInfo);
			//fadeDirectly(); //for testing purposes
			//write brightness so php part can read it :-)
			//writeCurrentBrightness();
			cout << "fading leds simultaneous finished" << endl;
			//print some debug info of the variables
			for (size_t ledsAvailable = 0; ledsAvailable < fadeInfo.leds.size(); ledsAvailable++) {
				cout << fadeInfo.leds[ledsAvailable].getColorCode() << " ";
				    //cout << leds[ledsAvailable].getPin() << " ";
				    //cout << leds[ledsAvailable].getIsColor() << " ";
				    //cout << leds[ledsAvailable].getCurrentBrightness() << " ";
				cout << fadeInfo.leds[ledsAvailable].getTargetBrightness() << endl;
			}
			//cout << "waitCounter: " << waitCounter << endl;
			cout << "mode: " << config.mode << endl;
			cout << "waitCounter: " << config.waitCounter << endl;

		}
		//if mode=1 and we haven't been before we want to start continious fadingc
		else if (config.mode == 1 && mode1ThreadActive == false)
		  	{
					//***** THREAD EXPLANATION ************************
			  		//for documentation about threads see:
			  		//https://computing.llnl.gov/tutorials/pthreads/
			  		//and about the pthread_create function see:
			  		//https://computing.llnl.gov/tutorials/pthreads/man/pthread_create.txt

			  		//it is recommended to not use a address as attribute because the value
			  		//may change until the thread is started (it's listed as bad example in the documentation.
			  		//but as we WANT to change it after the thread has started we're using it anyway.
			  		//if there are any problems we comment MODE_LIVE_MANIPULATING and everything is as its recommended.
			  		#ifndef MODE_LIVE_MANIPULATING
			  			//init the new thread, it can be adressed by mode1Thread, the fadeTimeUs variable will
			  			//be sent as attribute, the main function of the new thread is mode1
			  			threadErrorNumber = pthread_create(&mode1Thread, NULL, mode1 , (void *) &fadeInfo);
			  		#endif
			  		#ifdef MODE_LIVE_MANIPULATING
			  			//init the new thread, it can be adressed by mode1Thread
			  			//the fadeTimeUs variable will be sent as attribute
			  			//the main function of the new thread is mode1
			  			threadErrorNumber = pthread_create(&mode1Thread, NULL, mode1 , (void *) &fadeInfo);
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
		 //INVALID CODE

}


//***************************************TERMINATING FUNCTIONS***************************************

//function which is called if ctrl+c is pressed in order to close led-blaster normally
void ledBlasterTerminate(int dummy)
{
	//mode = 0; //so we won't have any problems with threads and so on
  printf("\nUser pressed Ctrl+C || SIGINT detected. Turn LEDs off.\n");
	// we want to turn all GPIOs of to avoid some strange stuff.
  fadeInfo.fadeTime = SIGINT_PIBLASTER_TERMINATE_TIME_VALUE;
  turnLedsOff(&fadeInfo); //turn all leds off in 1000ms = 1s so it won't take too long
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
	//mode = 0; //so we won't have any problems with threads and so on
  printf("\nSIGTERM detected. Turn LEDs off.\n");
	// we want to turn all GPIOs of to avoid some strange stuff.
  fadeInfo.fadeTime = SIGTERM_PIBLASTER_TERMINATE_FAST_TIME_VALUE;
  turnLedsOff(&fadeInfo); //turn all leds off in 50ms its fast. ;-). if that's not enough we may decrease it to 0
	//writeCurrentBrightness(); //useless but we'll save it anyway
	printf("terminate gpio \n");
	gpioTerminate(); //terminates GPIO (but doesn't necessarily turn all gpios off
	printf("close all threads \n");
	pthread_exit(NULL);
	printf("exit program. thank you. \n");
  	exit(1);
}
