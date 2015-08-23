//*************** R/W CURRENT BRIGHTNESS ************************

#include "currentBrightnessFileRW.hpp"
#include "led-blaster-pre.hpp"

#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include <string.h>
#include <string>



void readCurrentBrightness(void) {
	FILE *cbFile;
	const char *currentBrightnessFilename = "currentBrightness.led";

	cbFile = fopen(currentBrightnessFilename, "rb");
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
		cbFile = fopen(currentBrightnessFilename, "wb");
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
	}
}

//Write brightness to file
void writeCurrentBrightness (void) {
	FILE *cbFile;
	const char *currentBrightnessFilename = "currentBrightness.led";
	cbFile = fopen(currentBrightnessFilename, "wb");
	if (cbFile)
	{
		printf("Writing brightness to file\n");
		for (int i=0; i < 3; i++) {
			currentBrightness[i] = targetBrightness[i];
		}		
		fwrite(&currentBrightness[0], sizeof(unsigned char), 100, cbFile) ;
		fclose(cbFile);
	}
}		


