#ifndef _fade_hpp
#define _fade_hpp

#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include <stdint.h> //libary which includes uint8_t etc.ti



void fadeSuccessively(uint16_t delay);
void fadeSimultaneous(uint32_t time);
void fadeDirectly(uint16_t targetBrightness[]);
void turnLedsOff(uint32_t time); 

#endif //fade.hpp