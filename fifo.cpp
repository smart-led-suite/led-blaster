#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unistd.h>
#include <sys/time.h> //get time

#include "config.h"
#include "fifo.hpp"
#include "led-blaster-pre.hpp"
#include "led.hpp"


using namespace std;

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
    //  std::cout << "new line recieved:" << line << '\n';
			//exit is ok if there's no =
			if (line.compare("exit") == 0)
			{
				exit(0); //kill ledblaster
			}
      // check wheter to set, fade or change configuration (in this order because of speed)
      int applyMode = 0;
      if (line.compare(0, 1, "s") == 0)
      {
        applyMode = 0;
        line.erase(0,1);
      }
      else if (line.compare(0, 1, "f") == 0)
      {
        //fade colors
        applyMode = 1;
        line.erase(0,1);
      }
      else
      {
        std::cout << "not yet defined behavior" << '\n';
        applyMode = 2;
        ledBlasterTerminate(0);
      }
      //**************analyze incoming string ******************************
      //convert to stream again
      std::istringstream this_line(line);
      //after reading, store it in key[] and value[] arrays
      //key is the pin, value the new value
      int key[LED::getNumLeds()], value[LED::getNumLeds()];
      for (int pairNo = 0; pairNo < LED::getNumLeds(); pairNo++) {
        if (!this_line.eof())
        {
          string keyValueString, keyString, valueString;
          getline(this_line, keyValueString, ';');
          //std::cout << keyValueString << '\n';
          istringstream keyOrValueStream(keyValueString);
          getline(keyOrValueStream, keyString, ':');
          getline(keyOrValueStream, valueString, ':');
          key[pairNo] = stoi(keyString);
          value[pairNo] = stoi(valueString);
          //std::cout << "key/value: " << key[pairNo] << " " << value[pairNo] << endl;
        }
      }
      //std::cout << "applymode: " << applyMode << '\n';
      // now we can use key and value to do stuff
      // iterate through all leds possible
      for(auto const &iterator : LED::ledMap)
      {
        //now iterate through our gathered data
        for (int newData = 0; newData < LED::getNumLeds(); newData++)
        {
          //iterator fist is the pin of the LED map. check if it matches
          if (iterator.first == key[newData])
          {
            //we're in for some change!!
            //std::cout << "changing pin " << key[newData] << '\n';
            // now apply changes based on mode.
            if (applyMode == 0)
            {
              // if above conditions apply, we can fade directly or with short fadetime
              int difference = abs(iterator.second->getCurrentBrightness() - value[newData]);
              //decide wether to fade or to set the brightness
              if (difference >= FADE_SET_THRESHOLD) {
                iterator.second->fadeCancel();
                LED::setFadeTime(SHORT_FADE_TIME);
                iterator.second->setTargetBrightness(value[newData]);
                iterator.second->fadeInThread();
              } else {
                iterator.second->fadeCancel();
                iterator.second->setCurrentBrightness(value[newData]);
              }
            }
            else if (applyMode == 1)
            {
              //cancel previous fade and fade to targetBrightness
              LED::setFadeTime(LONG_FADE_TIME);
              iterator.second->fadeCancel();
              iterator.second->setTargetBrightness(value[newData]);
              iterator.second->fadeInThread();
            }
          }
        }

      }
		}
  }
}
