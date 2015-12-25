//*************** R/W CURRENT BRIGHTNESS ************************
//THE TWO READ/WRITE FUNCTIONS DO NOTHING RIGHT NOW
//theres a rewrite necessary but currently we dont use them anyway


#include "currentBrightnessFileRW.hpp"
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


void readConfig(void)
{
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
 	}
}

void writeCurrentBrightness (void) {
	ofstream myfile;
	myfile.open ("/var/www/html/brightness.csv");
  	if (myfile.is_open())
  	{
  		cout << "writing current brightness to file..." << endl;
			for (size_t ledsAvailable = 0; ledsAvailable < leds.size(); ledsAvailable++)
			{

				myfile << leds[ledsAvailable].getColorCode() << ";";
				//myfile << colors.second << ";";
				myfile << leds[ledsAvailable].getTargetBrightness() <<"\n";

		}
	}
	else
	{
   	cout << " current brightness file couldnt be opened" << endl;
   	}
	myfile.close();
	//cout << "finished." << endl;
	/* FILE *cbFile;
	const char *currentBrightnessFilename = "currentBrightness.txt";

	cbFile = fopen(currentBrightnessFilename, "r");
	if (cbFile)
	{
		//----- FILE EXISTS -----

		fread(&currentBrightness[0], sizeof(unsigned char), 4, cbFile);

		printf("read current brightness (w/r/g/b): %i %i %i %i\n", currentBrightness[0], currentBrightness[1], currentBrightness[2], currentBrightness[3]);

		fclose(cbFile);
	}
	else
	{
		//----- FILE NOT FOUND -----
		printf("File not found. Create new File\n");

		//Write new file
		cbFile = fopen(currentBrightnessFilename, "w");
		if (cbFile)
		{
			printf("set wrgb brightness to 0\n");
			currentBrightness[0] = 0;
			currentBrightness[1] = 0;
			currentBrightness[2] = 0;
			currentBrightness[3] = 0;

			fwrite(&currentBrightness[0], sizeof(unsigned char), 100, cbFile) ;

			fclose(cbFile);
		}
	} */
}

	void readCurrentBrightness(void) {
	}
