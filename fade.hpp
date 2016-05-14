#ifndef _fade_hpp
#define _fade_hpp

#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include <stdint.h> //libary which includes uint8_t etc.ti
#include "led-blaster-pre.hpp"

using namespace std;

int setFadeSteps(int *pwmSteps);
void fadeSuccessively(uint16_t delay);
void fadeSimultaneous(ledInformationStruct * led);
void fadeDirectly(ledInformationStruct * led);
void turnLedsOff(ledInformationStruct * led);

#endif //fade.hpp
