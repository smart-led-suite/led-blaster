#ifndef _led_blaster_pre_hpp
#define _led_blaster_pre_hpp

#include "fadeModes.hpp"
#include "config.h"
#include "currentBrightnessFileRW.hpp"
#include "init.hpp"
#include "modes.hpp"

#include <iostream>
#include <cstdlib>
#include <pigpio.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <pthread.h>
#include <stdint.h> //libary which includes uint8_t etc.

using namespace std;
//******************* VARIABLES ****************************

//pins		w, r, g, b
extern int pins[COLORS];
extern uint16_t targetBrightness[COLORS];
extern uint16_t currentBrightness[COLORS];
extern int fadeAlgorithm;
extern int realPWMrange;
extern int PWMrange;
extern int fadeDelayUs;
extern uint16_t mode;

//**************** FUNCTIONS ********************************


#endif
