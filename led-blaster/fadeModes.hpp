#ifndef _fadeModes_hpp
#define _fadeModes_hpp

#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include <stdint.h> //libary which includes uint8_t etc.ti



void fadeSuccessively(uint16_t delay, uint16_t targetBrightness[]);
void fadeSimultaneous(uint32_t delay);
void fadeDirectly(uint16_t targetBrightness[]);
void turnLedsOff(uint32_t time); 

#endif //fadeModes.hpp
