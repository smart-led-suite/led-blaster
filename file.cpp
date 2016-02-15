//*************** R/W CURRENT BRIGHTNESS ************************
//THE TWO READ/WRITE FUNCTIONS DO NOTHING RIGHT NOW
//theres a rewrite necessary but currently we dont use them anyway


#include "file.hpp"
#include "led-blaster-pre.hpp"
#include "led.hpp"

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <stdio.h>
#include <string.h>
#include <string>
#include <sstream>
#include <map>

#include <string>
#include <pthread.h>
#include <stdint.h> //libary which includes uint8_t etc.
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/stat.h>
#include <unistd.h>
#include <linux/stat.h>





//READING COLORS.CSV -> color config
void readColorConfig(void)
{
	//define filename
 	 const char *configFileName = (LED_WEB_SERVER_PATH + "colors.csv").c_str();
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
      string variables[2];
      //now we'll read the first two variables
      for (size_t variablesToRead = 0; variablesToRead < 2; variablesToRead++) {
        getline(bufferstream, variable_buffer, ';');
        #ifdef DEBUG
        cout << variable_buffer << endl;
        #endif
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
      #ifdef DEBUG
      cout << variables[0] << endl;
      cout << variables[1] << endl;
      std::cout << buffer << std::endl;
      #endif
 	  //  leds.push_back(le)
 	  }
      //close the config file
 	    configFile.close();
 	  }
 	else {
    //if opening of the file doesnt work, print an error
 	  cerr << "Unable to open file \"" << configFileName << "\", use defaults instead" << endl;
    //then well try to create it
      ofstream configFile (configFileName , ios::out);
      if (configFile.is_open()) {
        configFile << "w;25;#000000;#cfcfcf;weiß" << endl;
        configFile << "r;17;#ffffff;#b50333;rot" << endl;
        configFile << "g;18;#ffffff;#26a300;grün" << endl;
        configFile << "b;22;#ffffff;#0000ff;blau" << endl;
      }
      else
      {
        std::cerr << "no file could be created. check your rights. using defaults now." << std::endl;

      }
      //enter defaults for basic functionality
      leds.push_back(LED("w", 25, 0, 0, 0));
      leds.push_back(LED("r", 17, 0, 0, 0));
      leds.push_back(LED("g", 18, 0, 0, 0));
      leds.push_back(LED("b", 22, 0, 0, 0));
 	}
}

void writeCurrentBrightness (void) {
	ofstream myfile;
	myfile.open (LED_WEB_SERVER_PATH + "brightness.csv");
  	if (myfile.is_open())
  	{
  		cout << "writing current brightness to file..." << endl;
			for (size_t ledsAvailable = 0; ledsAvailable < leds.size(); ledsAvailable++)
			{
        //write colorcode and targetBrightness to file
				myfile << leds[ledsAvailable].getColorCode() << ";";
				//myfile << colors.second << ";";
				myfile << leds[ledsAvailable].getTargetBrightness() <<"\n";
		}
	}
	else
	{
   	cerr << " current brightness file couldnt be opened" << endl;
   	}
	myfile.close();
}

	void readTargetBrightness(void) {
    std::cout << "reading target Brightness" << std::endl;
    //define filename, consisting of LED_WEB_SERVER_PATH and the name and convert it to char
    //with the c_str function
    const char *brightnessFileName = (LED_WEB_SERVER_PATH + "brightness.csv").c_str();
    //open file
    ifstream brightnessFile (brightnessFileName , ios::in);
    if(brightnessFile.is_open())
    {
      //FILE IS OPEN
      string buffer;
      //read the brightnessFile line by line, save result in buffer
      while( getline(brightnessFile,buffer) )
      {
        //convert buffer to stream so we can call getline
        istringstream bufferstream(buffer);
        string variable_buffer;
        //this string array size is 2 because we'll store brightness and colorcode
        //from the brightness.csv file.
        string variables[2];
        //now we'll read the first two variables
        for (size_t variablesToRead = 0; variablesToRead < 2; variablesToRead++) {
          getline(bufferstream, variable_buffer, ';');
          #ifdef DEBUG
          cout << variable_buffer << endl;
          #endif
          //and save them in an array of strings
          variables[variablesToRead] = variable_buffer;
        }
        for (size_t currentLed = 0; currentLed < leds.size(); currentLed++) {
          //check if the current led matches the colorcode of the line of the brightnessFile we read
          if (leds[currentLed].getColorCode() == variables[0]) {
            leds[currentLed].setTargetBrightness(stoi(variables[1]));
          }
        }
        //some debug info
        /*#ifdef DEBUG
        cout << variables[0] << endl; //this is the colorcode
        cout << variables[1] << endl; //this is the current brightness (which will be the target brightness at init)
        std::cout << buffer << std::endl;
        #endif*/
      }
      //print new brightnesses
        for (size_t currentLed = 0; currentLed < leds.size(); currentLed++) {
          std::cout << "targetBrightness of " << leds[currentLed].getColorCode() << " is " << leds[currentLed].getTargetBrightness() << std::endl;
        }
        //close the config file
        brightnessFile.close();
      }
    else {
      //if opening of the file doesnt work, print an error
      cerr << "Unable to open file \"" << brightnessFileName << "\"" << endl;
      cout << "we'll ignore that we couldnt read brightness file because its not that important for basic functionality" << endl;
    }
	}
