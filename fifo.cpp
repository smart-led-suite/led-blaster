//============================================================================
// Name        : hello.cpp
// Author      : Sandesh
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
// ToDo	       :
//	(done)		 - new universal initPin
//	(done)		 - introduce header file so the functions can be placed below the main part
//	(done)		 - introduce file to save current luminances
//	(done)		 - introduce simultaneous fadeAlgorithm
//			 - introduce separate files
//			 - improve simultaneous algorithm
//			 - move currentBrightness read/write to own function
//			 - introduce other fadeModes (aka exp fade)
//	(done, untested) - introduce pipes (major)
//		 	 - introduce some other modes (fade w/ overlap of different colors)
//			 - introduce music mode
//			 - introduce config file w/ led pins etc.
//			 - tidying up code changes due to implementation of FIFO (BLOCKER)
//============================================================================

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unistd.h>

#include "config.h"
#include "fifo.hpp"
#include "led-blaster-pre.hpp"

// MOVE #defines into header


using namespace std;


bool assignValues (std::string key, std::string value, uint16_t * waitCounter)
{
  if (key.compare("time") == 0) {
    fadeTimeMs = std::stoi(value);
    return 0;
  }
	else if (key.compare("mode") == 0)
  {
    mode = std::stoi(value);
    return 0;
  }
	else if (key.compare("exit") == 0)
	{
		exit(0); //terminates LEDBlaster. function needs an int as argument which is useless (but necessary for signal() syntax
	}
	else if (key.compare("wait") == 0)
	{
		*waitCounter =  std::stoi(value);
		return 0;
	}
	else
	{
		bool commandFound = false;
		#ifdef DEBUG
			std::cout << "colorCode: " << key << std::endl;
			std::cout << "value: " << value << std::endl;
		#endif //DEBUG
		for (size_t ledsAvailable = 0; ledsAvailable < leds.size(); ledsAvailable++)
		{
			//if colorCode = key then we'll update the target brightness
			if(key.compare(leds[ledsAvailable].getColorCode()) == 0)
			{
				leds[ledsAvailable].setTargetBrightness(stoi(value));
				if (*waitCounter)
				{
					*waitCounter = *waitCounter - 1;
				}
				commandFound = true;

			}
		}
		if (commandFound == false) {
			std::cerr << "couldnt detect input" << std::endl;
			return 0;
		}
	}
}
//***********************************************************************************************
//********************************************** MAIN *********************************************
//*********************************************************************************************

void readFifo (uint16_t * waitCounter)
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
	          if (assignValues(key, value, waitCounter))
	          {
	            std::cerr << "configFile read error at" << line << std::endl;
	          }
				}
			}
	}
}
