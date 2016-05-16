//MODES
#include "modes.hpp"
#include "fade.hpp"
#include "config.h"
#include "file.hpp"
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


//continious fade w/random color (mode = 1)
// this function is initialized w/ a pointer in order to create a separate thread in main()
void *mode1(void* fadeInfo)
{
	 /* initialize random seed: */
  	srand (time(NULL));

	int fadeTime = 0;
	//it is recommended to not use a address as attribute because the value may change until the thread is started (it's listed as bad example in the documentation.)
	//but as we WANT to change it after the thread has started we're using it anyway. if there are any problems we comment MODE_LIVE_MANIPULATING and everything is as its recommended.
	#ifndef MODE_LIVE_MANIPULATING
		if (((ledInformationStruct *)fadeInfo)->fadeTime < 1) //if the fadeTime is 0 (i.e. no fade, just flickering) we'll set it to 1000 (default delay)
		{
			fadeTime = 10; //1000 as default
		}
		else
		{
			fadeTime = ((ledInformationStruct *)fadeInfo)->fadeTime; // turn fadeTimePointer pointer in a int and save it as fadeTime
		}
	#endif
	//turnLedsOff(1000);

	//if ctrl+c is pressed we want to terminate the gpios and close all open threads. therefore we'll want to catch the ctrl+c by the user
	//signal(SIGINT, terminateLedBlaster);
	signal(SIGINT, ledBlasterTerminate);
		//if the kill command is send (SIGTERM) we want to terminate the gpios and close all open threads a bit faster than if ctrl+c is detected
	signal(SIGTERM, ledBlasterTerminateFast);


	while(true) // until thread gets canceled by main thread
	{
		//generate random brightness for every color

		for (size_t ledsAvailable = 0; ledsAvailable < ((ledInformationStruct *)fadeInfo)->leds.size(); ledsAvailable++) {
			//exept white because that doesn't look nice.
			if (((ledInformationStruct *)fadeInfo)->leds[ledsAvailable].IsColor() == true)
			{
				//rand % 1000 creates numbers between 0 and 999, ...
				//% 1001 should create nubers between 0 and 1000
				((ledInformationStruct *)fadeInfo)->leds[ledsAvailable].setTargetBrightness(rand() % 1001);

				#ifdef MODE_LIVE_MANIPULATING
					//if the fadeTime is 0 (i.e. no fade, just flickering)
					//we'll set it to 1 (default delay)
					if (((ledInformationStruct *)fadeInfo)->fadeTime < 800)
					{
						fadeTime = 800;
					}
					else
					{
						// turn fadeTimePointer pointer in a int
						// and save it as fadeTime
						fadeTime = ((ledInformationStruct *)fadeInfo)->fadeTime;
					}
				#endif
        //start fading
        ((ledInformationStruct *)fadeInfo)->leds[ledsAvailable].fadeInThread();
				//fadeSimultaneous((ledInformationStruct *)fadeInfo);
			}
		}
    //wait for fade to finish
    for (size_t ledsAvailable = 0; ledsAvailable < ((ledInformationStruct *)fadeInfo)->leds.size(); ledsAvailable++) {
      if (((ledInformationStruct *)fadeInfo)->leds[ledsAvailable].IsColor() == true)
			{
        ((ledInformationStruct *)fadeInfo)->leds[ledsAvailable].fadeWait();
      }
    }

	}
  //INVALID CODE
	pthread_exit(NULL); //exit this thread

}
