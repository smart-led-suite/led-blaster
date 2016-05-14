#ifndef _currentBrightnessFileRW_hpp
#define _currentBrightnessFileRW_hpp
#include "led-blaster-pre.hpp"
//#include "led.hpp"

void readConfig(ledInformationStruct * ledInfo, configInformationStruct * config);
bool readColorConfig(ledInformationStruct * ledInfo);
void readTargetBrightness(ledInformationStruct * ledInfo);
void writeCurrentBrightness (ledInformationStruct * ledInfo);
bool assignConfigValues (std::string key, std::string value, ledInformationStruct * ledInfo, configInformationStruct * config);

#endif
