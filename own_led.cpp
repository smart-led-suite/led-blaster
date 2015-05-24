//============================================================================
// Name        : hello.cpp
// Author      : Sandesh
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <cstdlib>
#include <pigpio.h>
#include <stdio.h>
using namespace std;

#define PWM_CONFIG 0
//#define ONE_PIN_INTERACTIVE

#define GPIO_PIN 17
#define PWM_RANGE 1000
#define PWM_FREQUENCY 200
#define PWM_CONFIG 0
#define FADE_DELAY_US 1000

#define COLORS 4 //number of different leds, currently wrgb, maybe soon w/ autodetect
//pins		w, r, g, b
int pins[4] = {25, 17,18,22};
uint16_t targetBrightness[4];
int fadeAlgorithm = 0;
int realPWMrange = 0;
int PWMrange = 0;

bool initGeneral(void) {
	#ifdef ONE_PIN_INTERACTIVE
		cout << "enter pin (R/G/B/W || 17/18/22/25(or 27) :" << endl;
		cin >> pin;
		cout << "choosen pin: " << pin << endl;
	#endif
	
	if (gpioInitialise() < 0)
	{
	   cout << "pigpio initialisation failed." << endl;
	   return 0;
	}
	else
	{
	   cout << "pigpio initialised okay." << endl;
	   return 1;
	}
}

//bool initPin(int pin) {

	

int main(int argc, char* argv[]) {
	/****************************************************************************************
	they are chars and have to be converted to ints 
		first argument argv[0]: info about filename etc
		2nd: mode
		3rd: not used right now
		4/5/6/7: value for wrgb (only if mode=0)

	*/	
	
	int pin = 17;
	uint16_t targetBrightness[4];
	
	uint16_t mode = atoi(argv[1]);
	
	if(!initGeneral()) {
		return 0;
	}	
	
	if (!mode) {
		fadeAlgorithm = atoi(argv[2]);
		cout << "fade algorithm: " << fadeAlgorithm << endl;
		cout << "targetBrightness w/r/g/b: ";
		for (int i=0; i < COLORS; i++) {
			targetBrightness[i] = atoi(argv[i+3]);
			cout << targetBrightness[i] << " ";
		}
		cout << endl;
	} else if (mode==1) {
		pin = pins[atoi(argv[2])];
	}	
	
	
	
	//uint16_t targetBrightness = 0;
	int fadeDelayUs = FADE_DELAY_US;
		

	#if PWM_CONFIG
		int realRange = 0;
		int setRange = 0;
		int targetRange = 0;
		
		int targetFrequency = 0;
		int frequency = 0;
	
		cout << "enter target frequency :" << endl;
		cin >> targetFrequency;
		cout << "set frequency to: " << targetFrequency << endl;
		frequency = gpioSetPWMfrequency(pin, targetFrequency);
		cout << "range was set to closest frequency: " << frequency << endl;
		cout << "check frequency: " << gpioGetPWMfrequency(pin) << endl;	
		
		cout << "enter target range :" << endl;
		cin >> targetRange;
		cout << "set range to: " << targetRange << endl;
		realRange = gpioSetPWMrange(pin, targetRange);
		cout << "range was set to real range: " << realRange << endl;
		cout << "check range: " << gpioGetPWMrange(pin) << endl;
	#else
		#ifdef ONE_PIN_INTERACTIVE
		gpioSetPWMfrequency(pin, PWM_FREQUENCY);
		cout << "check frequency: " << gpioGetPWMfrequency(pin) << endl;
		gpioSetPWMrange(pin, PWM_RANGE);
		#endif
		//for all pins now done after defining as an output
	#endif		
	
	
	
	//cout << "enter mode (0 = targetBrightness[0] ; 1 = continiuos fade) :" << endl;
	//cin >> mode;
	cout << "chosen mode: " << mode << endl;
	
	if (mode==0) {
	#ifdef ONE_PIN_INTERACTIVE
		cout << "enter targetBrightness[0] (0 to " << PWM_RANGE << endl;
		cin >> targetBrightness[0];
		cout << "chosen targetBrightness[0]: " << targetBrightness[0] << endl;
	#endif	
	} else if (mode == 1) {
		cout << "enter fadeDelay (in us). default: " << FADE_DELAY_US << endl;
		cin >> fadeDelayUs;
		cout << "chosen fadeDelayUs: " << fadeDelayUs << endl;
	} else {
		cout << "choose correct mode!" << endl; // prints !!!Hello World!!!
		return 0;
	}
	
	for (int color = 0; color < COLORS; color++) {
		if (!gpioSetMode(pins[color], 1)) {
			cout << "set " << pins[color] << " to output" << endl;
		}
		gpioSetPWMfrequency(pins[color], PWM_FREQUENCY);
		cout << "check frequency: " << gpioGetPWMfrequency(pins[color]) << endl;
		gpioSetPWMrange(pins[color], PWM_RANGE);
		realPWMrange = gpioGetPWMrealRange(pins[color]);
		cout << "check real pwm range: " << realPWMrange << endl;
		PWMrange = gpioGetPWMrange(pins[color]);
		cout << "check pwm range: " << PWMrange << endl;
	}
	
	
	#ifdef ONE_PIN_INTERACTIVE
	if (!gpioSetMode(pin, 1)) {
		cout << "set " << pin << " to output" << endl;
	}			
	/*int realPWMrange = gpioSetPWMrange(pin, PWM_RANGE);
	cout << "pwm range set to: "  << endl;
	cout << realPWMrange  << endl;*/
	
	if (!gpioPWM(pin, 0)) {
		cout << "pwm set to 0 correctly" << endl;
	}
	#endif
	
	if (mode==0) {
	
		if (fadeAlgorithm==0) {
			for (int color = 0; color < COLORS; color++) {
				//cout << "chosen targetBrightness[0]: " << targetBrightness[0] << endl;		
				gpioPWM(pins[color], targetBrightness[color]);
				cout << "set pin: " << pins[color] << " to chosen brightness (" << targetBrightness[color] << "). Ctrl+C to exit" << endl;
			}	
			//while(1); //stay in the progamm and do nothing
			
		} else if (fadeAlgorithm==1) {
			cout << "fading leds chronologically..." << endl;
			uint16_t currentBrightness[4] = {0,0,0,0}; 
			for (int color = 0; color < COLORS; color++) {
				for (currentBrightness[color]; currentBrightness[color] < targetBrightness[color]; currentBrightness[color]++) {
					//cout << "chosen targetBrightness[0]: " << targetBrightness[0] << endl;		
					gpioPWM(pins[color], currentBrightness[color]);
					gpioDelay(fadeDelayUs);
					//cout << "set pin: " << pins[color] << " to chosen brightness (" << targetBrightness[color] << "). Ctrl+C to exit" << endl;
				}	
			}
		}
		cout << "leds stay on for 10s..." << endl;
		gpioDelay(10000000);
	}
	
	
	
	if (mode==1) {
		uint16_t currentBrightnessSingle = 0;
		cout << "start continuos fade. exit with ctrl+c" << endl;
		while(1) {
				while(currentBrightnessSingle < realPWMrange) {
					currentBrightnessSingle++;
					gpioPWM(pin, currentBrightnessSingle);
					gpioDelay(fadeDelayUs);
					//cout << currentBrightnessSingle << endl;
				}
				while(currentBrightnessSingle > 0) {
					currentBrightnessSingle--;
					gpioPWM(pin, currentBrightnessSingle);
					gpioDelay(fadeDelayUs);
					//cout << currentBrightnessSingle << endl;
				}
			}
	}
	
	
	
	
	cout << "end of program. turn all leds off" << endl; // prints !!!Hello World!!!
	for (int color = 0; color < COLORS; color++) {
		gpioWrite(pins[color], 0);
	}	
	gpioTerminate();
	cout << "gpio closed" << endl;
	return 0;
}
