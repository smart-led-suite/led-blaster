#ifndef _currentBrightnessFileRW_hpp
#define _currentBrightnessFileRW_hpp
#include "led-blaster-pre.hpp"
//#include "led.hpp"

void readConfig(configInformationStruct * config);
bool readColorConfig(void);
void readTargetBrightness(void);
void writeCurrentBrightness (void);
bool assignConfigValues (std::string key, std::string value, configInformationStruct * config);

#endif
