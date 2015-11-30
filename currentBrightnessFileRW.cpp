//*************** R/W CURRENT BRIGHTNESS ************************
//THE TWO READ/WRITE FUNCTIONS DO NOTHING RIGHT NOW
//theres a rewrite necessary but currently we dont use them anyway


#include "currentBrightnessFileRW.hpp"
#include "led-blaster-pre.hpp"

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <pigpio.h>
#include <stdio.h>
#include <string.h>
#include <string>
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





void writeCurrentBrightness (void) {
	ofstream myfile;
	myfile.open ("/var/www/html/example.csv");
  	if (myfile.is_open()) 
  	{
  		cout << "writing current brightness to file..." << endl;
		for(auto const &colors : pin)
		{		
			//*******maps explanation**************
	  		//write the target brightness to the pin
	  		//colors.second marks the second row in the pin map
	  		//ledsTarget[colors.first] is the brightness which belongs to the specific color
			//colors.first is in this case the name of the color which is also an ID for the targetBrightness
		
				myfile << colors.first << ";";
				myfile << colors.second << ";";
				myfile << ledsTarget[colors.first] <<"\n";
				
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


