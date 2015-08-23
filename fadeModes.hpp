#ifndef _fadeModes_hpp
#define _fadeModes_hpp

#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include <stdint.h> //libary which includes uint8_t etc.



void fadeSuccessively(uint16_t delay, uint16_t targetBrightness[]);
void fadeSimultaneous(uint16_t delay, uint16_t targetBrightness[]);
void fadeDirectly(uint16_t targetBrightness[]);


#endif //fadeModes.hpp
