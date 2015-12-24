#include <iostream>
//#include <map>
#include <stdint.h> //libary which includes uint16_t etc.
#include "config.h"
#include "led.hpp"

//#include <stdio.h>
#include <pigpio.h>
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
uint16_t LED::getCurrentBrightenss()
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
void LED::setCurrentBrightenss(uint16_t new_cBrightness)
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
  gpioPWM(pin, brightness);
}




LED::LED(std::string led_colorcode, uint16_t led_pin, bool led_isColor, uint16_t led_currentBrightness, uint16_t led_targetBrightness)
{
  colorcode = led_colorcode;
  pin = led_pin;
  isColor = led_isColor;
  currentBrightness = led_currentBrightness;
  targetBrightness = led_targetBrightness;
}


int main(int argc, char const *argv[]) {
  //some class init
  LED led("w", 25, false, 0, 20);
  std::vector<LED> leds;
  //leds.push_back(LED("r", 23, false, 0, 20));
//  cout << leds[0].getPin() << endl;
//  cout << leds.size() << endl;
  //file testing
  //define filename
 	const char *configFileName = "/var/www/html/colors.csv";
  //open file
  ifstream configFile (configFileName , ios::in);
 	if(configFile.is_open())
  {
    //FILE IS OPEN
    string buffer;
    //read the configFile line by line, save result in buffer
 	  while( getline(configFile,buffer) )
    {
      //convert buffer to stream so we can call getline
      istringstream bufferstream(buffer);
      string variable_buffer;
      //this string has the size 2 because in led-blaster we only need
      //variable 1 and 2 from the colors.csv file.
      string variables[1];
      //now we'll read the first two variables
      for (size_t variablesToRead = 0; variablesToRead < 2; variablesToRead++) {
        getline(bufferstream, variable_buffer, ';');
        cout << variable_buffer << endl;
        //and save them in an array of strings
        variables[variablesToRead] = variable_buffer;
      }
      //for now we'll just use the color code to specify wheter its a color or not
      //if we find a 'w' we'll assume its white and therefore no color attached
      bool ledIsColor = true;
      if (variables[0].find("w") != std::string::npos)
      {
        ledIsColor = false;
      }
      //add this as a new element in leds vector element
      leds.push_back(LED(variables[0], stoi(variables[1],nullptr), ledIsColor, 0, 0));
      cout << variables[0] << endl;
      cout << variables[1] << endl;
      std::cout << buffer << std::endl;
 	  //  leds.push_back(le)
 	  }
      //close the config file
 	    configFile.close();
 	  }
 	else {
    //if opening of the file doesnt work, print an error
 	  cout << "Unable to open file \"" << configFileName << "\"" << endl;
 	} //);

  //print the leds vector
  for (size_t ledsAvailable = 0; ledsAvailable < leds.size(); ledsAvailable++) {
    cout << leds[ledsAvailable].getColorCode() << " ";
    cout << leds[ledsAvailable].getPin() << " ";
    cout << leds[ledsAvailable].getIsColor() << " ";
    cout << leds[ledsAvailable].getCurrentBrightenss() << " ";
    cout << leds[ledsAvailable].getTargetBrightness() << endl;
  }

/*  cout << led.getPin() << endl;
  cout << led.getIsColor() << endl;
  cout << led.getCurrentBrightenss() << endl;
  cout << led.getTargetBrightness() << endl;
  led.setPin(25);
  led.setIsColor(true);
  //led.writeBrightnessToPin(200);
  cout << led.getPin() << endl;
  cout << led.getIsColor() << endl;
  cout << led.getCurrentBrightenss() << endl;
  cout << led.getTargetBrightness() << endl;*/
  return 0;
}
