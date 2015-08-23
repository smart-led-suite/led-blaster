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
	cout << "continious fade mode. set pin: 17/18/22/25 R/G/B/W: " << endl;
	int pin = 25;
	//cin >> pin;
	cout << "value of mode is: " << mode << endl;
	cout << "start continuos fade on pin " << pin << ". exit with mode = 0" << endl;
	cout << "Enter your configuration: " << endl ; //surprisingly, without endl it prints nothing (at the begin of the main loop the same command works)
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


