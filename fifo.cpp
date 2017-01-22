#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unistd.h>

#include "config.h"
#include "fifo.hpp"
#include "led-blaster-pre.hpp"
#include "led.hpp"


using namespace std;


bool assignValues (std::string key, std::string value, configInformationStruct * config)
{
  int valueAsInt = stoi(value);
  if (key.compare("time") == 0) {
    //*fadeTimeMs = valueAsInt;
    LED::setFadeTime(valueAsInt);
    return 0;
  }
	else if (key.compare("mode") == 0)
  {
    switch (valueAsInt) {
      case 0:
        config->mode = 0;
        break;
      case 1:
        config->mode = 1;
        break;
      default:
        config->mode = 0;
        break;
    }
    return 0;
  }
	else if (key.compare("exit") == 0)
	{
		exit(0); //terminates LEDBlaster. function needs an int as argument which is useless (but necessary for signal() syntax
	}
	else if (key.compare("wait") == 0)
	{
		config->waitCounter =  valueAsInt;
		return 0;
	}
	else
	{
		bool commandFound = false;
		#ifdef DEBUG
			std::cout << "colorCode: " << key << std::endl;
			std::cout << "value: " << valueAsInt << std::endl;
		#endif //DEBUG
    for(auto const &iterator : LED::ledMap)
    {
			//if colorCode = key then we'll update the target brightness
			if(key.compare(iterator.second->getColorCode()) == 0)
			{
        //value needs to be greater than 0 and mustn't be larger than the number of steps
        if (valueAsInt >= 0 && valueAsInt <= LED::getPwmSteps())
        {
          if (config->waitCounter == 0 && LED::getFadeTime() <= 1)
          {
            // if above conditions apply, we can fade directly
            iterator.second->setCurrentBrightness(valueAsInt);
          }
          else
          {
            iterator.second->setTargetBrightness(valueAsInt);
          }
        }
        else
        {
          std::cout << "according to todays physics, a negative brightness is not possible. ;)" << std::endl;
          std::cout << "please set a brightness between 0 and " << LED::getPwmSteps() << std::endl;
        }
				if (config->waitCounter)
				{
					config->waitCounter = config->waitCounter - 1;
				}
				commandFound = true;
			}
		}
		if (commandFound == false) {
			std::cerr << "couldnt detect input" << std::endl;
			return 0;
		}
	}
  return 0;
}
//***********************************************************************************************
//********************************************** MAIN *********************************************
//*********************************************************************************************

void readFifo (configInformationStruct * config)
{
	//open file
	//this blocks the function until Something can be read from the FIFO
	ifstream fifo (FIFO_FILE , ios::in);
	if(fifo.is_open())
  {
    //FILE IS OPEN
    //read it line by line
		//var to store a single line
		std::string line;
    while( std::getline(fifo, line) )
    {
			//convert to stream again
      std::istringstream this_line(line);
			//exit is ok if there's no =
			if (line.compare("exit") == 0)
			{
				exit(0); //kill ledblaster
			}
      //now we'll save the key
      std::string key;
      //values are seperated by '=' in the file
      if( std::getline(this_line, key, '=') )
      {
				//and save the value
        std::string value;
        if( std::getline(this_line, value) )
          std::cout << "key/value recieved " << key << " " << value << std::endl;
          if (assignValues(key, value, config))
          {
            std::cerr << "fifo read error at" << line << std::endl;
          }
			}
      std::cout << "apply values" << '\n';
      applyNewValues(config);
		}
  }
}
