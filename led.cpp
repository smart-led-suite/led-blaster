#include <iostream>
//#include <map>
#include <stdint.h> //libary which includes uint16_t etc.
#include "config.h"
#include "led.hpp"
#include "init.hpp"

//#include <stdio.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>


using namespace std;
//getter
std::string LED::getColorCode()
{
  return (colorcode);
}
uint16_t LED::getPin()
{
  return pin;
}
bool LED::getIsColor()
{
  return isColor;
}
uint16_t LED::getCurrentBrightness()
{
  return currentBrightness;
}
uint16_t LED::getTargetBrightness()
{
  return targetBrightness;
}

//setter
void LED::setColorCode(std::string new_colorcode)
{
  colorcode = new_colorcode;
}
void LED::setPin(uint16_t newpin)
{
  pin = newpin;
}
void LED::setIsColor(bool newisColor)
{
  isColor = newisColor;
}
void LED::setCurrentBrightness(uint16_t new_cBrightness)
{
  currentBrightness = new_cBrightness;
}
void LED::setTargetBrightness(uint16_t new_tBrightness)
{
  targetBrightness = new_tBrightness;
}
//functions
void LED::writeBrightnessToPin(uint16_t brightness)
{
  //gpioPWM(pin, brightness);
}




LED::LED(std::string led_colorcode, uint16_t led_pin, bool led_isColor, uint16_t led_currentBrightness, uint16_t led_targetBrightness)
{
  colorcode = led_colorcode;
  pin = led_pin;
  isColor = led_isColor;
  currentBrightness = led_currentBrightness;
  targetBrightness = led_targetBrightness;
  //initializes each pin. returns 0 if everything went ok
  std::cout << "initializes color  \"" << colorcode << "\"" << std::endl;
  if(initPin(pin) != 0) {
    //print that there has been an error if this happens (very unlikely)
    cout << "error in initPin " << pin;
    cout << "which is used by color " << colorcode << endl;
    //and exit the program
    exit(EXIT_FAILURE);
  }
}
