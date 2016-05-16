//*************** R/W CURRENT BRIGHTNESS ************************
//THE TWO READ/WRITE FUNCTIONS DO NOTHING RIGHT NOW
//theres a rewrite necessary but currently we dont use them anyway


#include "file.hpp"
#include "led-blaster-pre.hpp"
#include "led.hpp"

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <pigpio.h>
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

#include "config.h"

//path to the apache server
std::string serverPath = "/var/www/html/";

//own private function so we have the config values at the top
bool assignConfigValues (std::string key, std::string value, ledInformationStruct * ledInfo, configInformationStruct * config)
{
  if (key.compare("time") == 0) {
    int valueAsInt = std::stoi(value);
    LED::setFadeTime(valueAsInt);
    return 0;
  }
  if (key.compare("server_path") == 0)
  {
    config->serverPath = value;
    return 0;
  }
  return 1;
}

//read general config from config file
void readConfig(ledInformationStruct * ledInfo, configInformationStruct * config)
{
  const char *configFileName = "/etc/led-blaster.ledconfig";
    //open file
  ifstream configFile (configFileName , ios::in);
  if(configFile.is_open())
    {
    //FILE IS OPEN
    std::string line;
    //read it line by line
    while( std::getline(configFile, line) )
    {
      //convert to stream again
      std::istringstream is_line(line);
      //detect // and # at the beginning of the line as comment
      if (line.compare(0,1,"#") == 0 || line.compare(0,2,"//") == 0)
      {
        continue; //go to the next line
      }
      //now we'll save the key
      std::string key;
      //values are seperated by '=' in the file
      if( std::getline(is_line, key, '=') )
      {
        //and save the value
        std::string value;
        if( std::getline(is_line, value) )
          #ifdef DEBUG
          std::cout << "key/value recieved " << key << " " << value << std::endl;
          #endif
          if (assignConfigValues(key, value, ledInfo, config))
          {
            std::cerr << "configFile read error at" << line << std::endl;
          }
      }
    }
    #ifdef DEBUG
    std::cout << "fadetime: " << LED::getFadeTime() << std::endl;
  //  std::cout << "server: " << serverPath << std::endl;
    #endif
    configFile.close();
  }
  else
   {
     std::cerr << "config file couldnt be opened, try creating it and use defaults" << std::endl;
     //then well try to create it
       ofstream configFile (configFileName , ios::out);
       if (configFile.is_open()) {
         //write default settings
         configFile << "#config file for led-blaster, source found usually at /opt/led-blaster" << endl;
         configFile << "#some config values are stored here" << endl << endl;
         configFile << "#syntax: " << endl;
         configFile << "#   key=value " << endl;
         configFile << "#   no whitespaces allowed, only one key/value pair per line! " << endl;
         configFile << "#   lines started with '#' and '//' are comments" << endl << endl;
         configFile << "#time which is needed to fade (in ms)" << endl;
         configFile << "time=" << FADE_TIME_MS <<  endl;
         configFile << "#path to the apache server with the colors/brightness.csv config files" << endl;
         configFile << "server_path=/var/www/html/" << endl;
         configFile.close();
       }
       else
       {
         std::cerr << "no file could be created. check your rights. using defaults now." << std::endl;

       }
       ledInfo->fadeTime = FADE_TIME_MS;

   }
}



//READING COLORS.CSV -> color config
bool readColorConfig(ledInformationStruct * ledInfo)
{
  //struct ledInformationStruct fadeInfo;
  //struct ledInformationStruct ledInfo;
	//define filename
 	 const char *configFileName = (serverPath + "colors.csv").c_str();
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
    //  leds.push_back(LED(variables[0], stoi(variables[1],nullptr), ledIsColor, 0, 0));
      ledInfo->leds.push_back(LED(variables[0], stoi(variables[1],nullptr), ledIsColor, 0, 0));
      #ifdef DEBUG
      cout << variables[0] << endl;
      std::cout << "is color: " << ledIsColor << std::endl;
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
    /*  leds.push_back(LED("w", 25, 0, 0, 0));
      leds.push_back(LED("r", 17, 0, 0, 0));
      leds.push_back(LED("g", 18, 0, 0, 0));
      leds.push_back(LED("b", 22, 0, 0, 0));*/
      ledInfo->leds.push_back(LED("w", 25, 0, 0, 0));
      ledInfo->leds.push_back(LED("r", 17, 0, 0, 0));
      ledInfo->leds.push_back(LED("g", 18, 0, 0, 0));
      ledInfo->leds.push_back(LED("b", 22, 0, 0, 0));
      return 1;
 	}
  return 0;
}

void writeCurrentBrightness (ledInformationStruct * ledInfo) {
	ofstream myfile;
	myfile.open (serverPath + "brightness.csv");
  	if (myfile.is_open())
  	{
  		cout << "writing current brightness to file..." << endl;
			for (size_t ledsAvailable = 0; ledsAvailable < ledInfo->leds.size(); ledsAvailable++)
			{
        //write colorcode and targetBrightness to file
				myfile << ledInfo->leds[ledsAvailable].getColorCode() << ";";
				//myfile << colors.second << ";";
				myfile << ledInfo->leds[ledsAvailable].getTargetBrightness() <<"\n";
		}
	}
	else
	{
   	cerr << " current brightness file couldnt be opened" << endl;
   	}
	myfile.close();
}

	void readTargetBrightness(ledInformationStruct * ledInfo) {
    std::cout << "reading target Brightness" << std::endl;
    //define filename, consisting of serverPath and the name and convert it to char
    //with the c_str function
    const char *brightnessFileName = (serverPath + "brightness.csv").c_str();
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
        string variables[1];
        //now we'll read the first two variables
        for (size_t variablesToRead = 0; variablesToRead < 2; variablesToRead++) {
          getline(bufferstream, variable_buffer, ';');
          #ifdef DEBUG
          cout << variable_buffer << endl;
          #endif
          //and save them in an array of strings
          variables[variablesToRead] = variable_buffer;
        }
        for (size_t currentLed = 0; currentLed < ledInfo->leds.size(); currentLed++) {
          //check if the current led matches the colorcode of the line of the brightnessFile we read
          if (ledInfo->leds[currentLed].getColorCode() == variables[0]) {
            ledInfo->leds[currentLed].setTargetBrightness(stoi(variables[1]));
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
        for (size_t currentLed = 0; currentLed < ledInfo->leds.size(); currentLed++) {
          std::cout << "targetBrightness of " << ledInfo->leds[currentLed].getColorCode() << " is " << ledInfo->leds[currentLed].getTargetBrightness() << std::endl;
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
