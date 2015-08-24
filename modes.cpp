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
#include <signal.h>
#include <string.h>
#include <string>
#include <pthread.h>
#include <stdint.h> //libary which includes uint8_t etc.
using namespace std;


//continious fade (mode = 1)
// this function is initialized w/ a pointer in order to create a separate thread in main()
void *modeContiniousFade(void*) 
{
	
	uint16_t currentBrightnessSingle = 0;
	int pin = 25;
	
	//if ctrl+c is pressed we want to terminate the gpios and close all open threads. therefore we'll want to catch the ctrl+c by the user
	//signal(SIGINT, terminateLedBlaster);
	signal(SIGINT, ledBlasterTerminate);
	
	cout << "start continuos fade on pin " << pin << ". exit with mode = 0 or Ctrl+C, DO NOT CHANGE ANY OTHER VALUE OR YOU HAVE TO REBOOT YOUR PI" << endl;
	while(mode == 1) 
	{
		while(currentBrightnessSingle < realPWMrange) 
		{
			currentBrightnessSingle++;
			gpioPWM(pin, currentBrightnessSingle);
			gpioDelay(fadeDelayUs);
			//cout << currentBrightnessSingle << endl;
		}
		while(currentBrightnessSingle > 0) 
		{
			currentBrightnessSingle--;
			gpioPWM(pin, currentBrightnessSingle);
			gpioDelay(fadeDelayUs);
			//cout << currentBrightnessSingle << endl;
		}
	}
	mode = 0;
	pthread_exit(NULL);
		
}


