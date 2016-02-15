#ifndef _led_blaster_pre_hpp
#define _led_blaster_pre_hpp

//THE TWO READ/WRITE FUNCTIONS DO NOTHING RIGHT NOW
//theres a rewrite necessary but currently we dont use them anyway

#include "fade.hpp"
#include "config.h"
#include "file.hpp"
#include "init.hpp"
#include "modes.hpp"

#include <map>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <stdio.h>
#include <string.h>
#include <string>
#include <pthread.h>
#include <stdint.h> //libary which includes uint8_t etc.
#include "led.hpp"

using namespace std;
//******************* VARIABLES ****************************
extern std::string LED_WEB_SERVER_PATH;
extern std::vector<LED> leds;
extern int fadeTimeMs;
extern int fadeAlgorithm;
extern int realPWMrange;
extern int PWMrange;
extern int fadeDelayUs;
extern uint16_t mode;

extern map < string, int> pin;
extern map < string, int> ledsTarget;
extern map < string, int> ledsCurrent;

//**************** FUNCTIONS ********************************
void ledBlasterTerminate(int dummy); //function which terminates the program safely. needs the dummy variable to be called by the sigHandler!
void ledBlasterTerminateFast(int dummy); //same but FASTER
#endif
