//============================================================================
// Name        : hello.cpp
// Author      : Sandesh
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
// ToDo	       : 
//	(done)	 - new universal initPin
//	(done)	 - introduce header file so the functions can be placed below the main part
//		 - introduce file to save current luminances
//		 - introduce simultaneous fadeAlgorithm
//		 - introduce pipes (major)
//		 - introduce some other modes (fade w/ overlap of different colors)
//		 - introduce music mode
//		 - introduce config file w/ led pins etc. 
//============================================================================

#include </var/www/own_led02.hpp>
using namespace std;


//pins		w, r, g, b
int pins[4] = {25, 17,18,22};
uint16_t targetBrightness[4];
int fadeAlgorithm = 0;
int realPWMrange = 0;
int PWMrange = 0;
int fadeDelayUs = FADE_DELAY_US;

uint16_t currentBrightness[4];

//***********************************************************************************************
//********************************************** MAIN *********************************************
//*********************************************************************************************	

int main(int argc, char* argv[]) {
	/****************************************************************************************
	they are chars and have to be converted to ints 
		first argument argv[0]: info about filename etc
		2nd: mode
		3rd: not used right now
		4/5/6/7: value for wrgb (only if mode=0)

	*/	
	
	
	//OPEN CONFIG FILE IN OUR APPLICAITONS DIRECTORY OR CREATE IT IF IT DOESN'T EXIST
	FILE *cbFile;
	string a("test");
	const char *currentBrightnessFilename = "currentBrightness.led";

	cbFile = fopen(currentBrightnessFilename, "rb");
	if (cbFile)
	{
		//----- FILE EXISTS -----
		fread(&currentBrightness[0], sizeof(unsigned char), 4, cbFile);

		printf("File opened, some byte values: %i %i %i %i\n", currentBrightness[0], currentBrightness[1], currentBrightness[2], currentBrightness[3]);

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
	
	
	
	int pin = 17; //pin used for continious fade on 1 pin
	//uint16_t targetBrightness[4];
	uint16_t mode = atoi(argv[1]);
	
	//init pwm
	if(!initGeneral()) {
		return 0;
	}
	
	
	//initialize all pins
	for (int color = 0; color < COLORS; color++) {
		if(!initPin(pins[color], mode)) {
			cout << "error in initPin " << pins[color] << endl;
			return 0;
		}	
	}
	
	//init variables to use with the interactive live input
	char variable[5];
	//command names
	char modeName[] = "mode";
	char waitName[] = "wait";
  	char redb[] = "redb";
  	char grnb[] = "grnb";
  	char blub[] = "blub";
  	char whtb[] = "whtb";
  	char exit[] = "exit";
  	char usel[] = "usel"; //useless but necessary
  	
  	//values
  	uint16_t wait=0;
  	uint16_t value = 0;
  	uint16_t brightness;
  	uint16_t waitCounter = 0;
 	// char* pointer = NULL,  nl;
  	int scanSuccess = 0;
  	
  	/*char *lineptr = NULL, nl;
  	int n, servo;
  	float value;*/
	
//	int mode = 0;
	//int targetBrightness[4];
	printf ("Welcome to your LED fade program \n");
	printf ("enter exit = 1 to exit \n");
	printf ("enter redb / grnb / blub / whtb = value to set brightness for a specific color \n");
	printf ("Enter mode = 0/1 to set mode \n");
	printf ("Enter wait = 0/1 (1 to wait until every color has been updated, then fade simutaneous \n");
	
	const char *configFilename = "config.blabla";
	FILE *confFile;
	
	char fileInputString[5] = "    ";
	char fileInputValue[20] = "   ";
	
	confFile = fopen(configFilename, "r");
	
	
	if (confFile)
	{
		//----- FILE EXISTS -----
		//while (fileInputString[0]==' ') {
		fread(&fileInputString, sizeof(char), 4, confFile);
		//}
		fileInputString[5] = ' ';
		cout << "File opened, print string " << fileInputString << endl;
		
		fread(&fileInputValue, sizeof(char), 20, confFile);
		cout << "File opened, print string n2 " << fileInputValue << endl;
		
		//now change modes
		 if (strcmp(exit, fileInputString)==0) {
	  		printf("exit program. thank you. \n");
	  	return 0;
	  } else if (strcmp(modeName, fileInputString)==0) {
	  	sscanf (fileInputValue, " = %d", &value);
	  	cout << value << endl; 
	  	mode = value;
	  } else if (strcmp(waitName, fileInputString)==0) {
	  	sscanf (fileInputValue, " = %d", &value);
	  	cout << value << endl; 
	  	mode = value;
	  	wait = value;
	  } else if (strcmp(whtb, fileInputString)==0) {
	  	sscanf (fileInputValue, " = %d", &value);
	  	cout << value << endl; 
	  	mode = value;
	  	targetBrightness[0] = value;
	  	if (!wait) {
	  		fadeSuccessively(fadeDelayUs, targetBrightness);
	  	} else { 
	  		waitCounter++;
	  	}	
	  } else if (strcmp(redb, fileInputString)==0) {
	  	sscanf (fileInputValue, " = %d", &value);
	  	cout << value << endl; 
	  	mode = value;
	  	targetBrightness[1] = value;
	  	if (!wait) {
	  		fadeSuccessively(fadeDelayUs, targetBrightness);
	  	} else { 
	  		waitCounter++;
	  	}
	  } else if (strcmp(grnb, fileInputString)==0) {
	  	targetBrightness[2] = value;
	  	sscanf (fileInputValue, " = %d", &value);
	  	cout << value << endl; 
	  	mode = value;
	  	if (!wait) {
	  		fadeSuccessively(fadeDelayUs, targetBrightness);
	  	} else { 
	  		waitCounter++;
	  	}
	  } else if (strcmp(blub, fileInputString)==0) {
	 	sscanf (fileInputValue, " = %d", &value);
	  	cout << value << endl; 
	  	mode = value;
	  	targetBrightness[3] = value;
	  	if (!wait) {
	  		fadeSuccessively(fadeDelayUs, targetBrightness);
	  	} else { 
	  		waitCounter++;
	  	}	
	  }

		fclose(confFile);
		printf("clear file\n");

		//Write new file
		confFile = fopen(configFilename, "w");
		if (confFile)
		{
			/*printf("write some values \n");
			fileInputString[0] = 'c';

			fwrite(&fileInputString, sizeof(fileInputString), 1, confFile) ;*/

			fclose(confFile);
		}
		//return 0;
	}
	else
	{
		//----- FILE NOT FOUND -----
		printf("File not found. Create new File\n");

		//Write new file
		confFile = fopen(configFilename, "w");
		if (confFile)
		{
			/*printf("write some values \n");
			fileInputString[0] = 'c';

			fwrite(&fileInputString, sizeof(fileInputString), 1, confFile) ;*/

			fclose(confFile);
		}
	}
	
	while(true) {
	
	
	  printf ("Enter your configuration: ");
	  scanSuccess = scanf ("%s = %d",&variable, &value);  
	  printf("variable changed: %s set to %d. \n", variable, value);
	  printf("return value: %d \n", scanSuccess);
	  cout << variable << endl;
	 // cout << waitName << endl;
	  if (strcmp(exit, variable)==0) {
	  	printf("exit program. thank you. \n");
	  	return 0;
	  } else if (strcmp(modeName, variable)==0) {
	  	mode = value;
	  } else if (strcmp(waitName, variable)==0) {
	  	wait = value;
	  } else if (strcmp(whtb, variable)==0) {
	  	targetBrightness[0] = value;
	  	if (!wait) {
	  		fadeSuccessively(fadeDelayUs, targetBrightness);
	  	} else { 
	  		waitCounter++;
	  	}	
	  } else if (strcmp(redb, variable)==0) {
	  	targetBrightness[1] = value;
	  	if (!wait) {
	  		fadeSuccessively(fadeDelayUs, targetBrightness);
	  	} else { 
	  		waitCounter++;
	  	}
	  } else if (strcmp(grnb, variable)==0) {
	  	targetBrightness[2] = value;
	  	if (!wait) {
	  		fadeSuccessively(fadeDelayUs, targetBrightness);
	  	} else { 
	  		waitCounter++;
	  	}
	  } else if (strcmp(blub, variable)==0) {
	  	targetBrightness[3] = value;
	  	if (!wait) {
	  		fadeSuccessively(fadeDelayUs, targetBrightness);
	  	} else { 
	  		waitCounter++;
	  	}	
	  }
	  
	  if (waitCounter>3) {
	  	fadeSimultaneous(fadeDelayUs, targetBrightness);
	  	waitCounter=0;
	  }
	  cout << waitCounter << endl;
	  cout << mode << endl;
	  cout << wait << endl;
	  cout << targetBrightness[0] << endl;
	  cout << targetBrightness[1] << endl;
	  cout << targetBrightness[2] << endl;
	  cout << targetBrightness[3] << endl;
	  /*	
	  printf("sizeof %d", sizeof(variable));
	  for (int i=0; i < sizeof(variable); i++) {
	  	printf("value of variable[%d] is %s", i, variable[i]);
	  }*/
	}
		
		
	
	
	
	
	
	if (mode==0) {  //fade to targetLuminance
		fadeAlgorithm = atoi(argv[2]);
		cout << "fade algorithm: " << fadeAlgorithm << endl;
		cout << "targetBrightness w/r/g/b: ";
		for (int i=0; i < COLORS; i++) {
			targetBrightness[i] = atoi(argv[i+3]);
			cout << targetBrightness[i] << " ";
		}
		cout << endl;
	} else if (mode==1) {
		pin = pins[atoi(argv[2])];
	}	
	
	
	
	//uint16_t targetBrightness = 0;
	
	//while (1) {
	/*	cout << "entering loop. white=2000 to exit. color w/r/g/b" << endl;
		for (int color; color < COLORS; color++) {
			targetBrightness[color]=5000;
			cout << "new targetBrightness: ";	
			while (targetBrightness[color]==5000) {
				cin >> targetBrightness[color];
			}	 
		/*	cout << "new targetBrightness w: or 2000 to exit";
			cin >> targetBrightness[0];
			cout << "new targetBrightness r: ";
			cin >> targetBrightness[1];
			cout << "new targetBrightness g: ";
			cin >> targetBrightness[2];
			cout << "new targetBrightness b: ";
			cin >> targetBrightness[3];
		}
	if (targetBrightness[0]==2000) {
		cout << "will exit now" << endl;
		return 0;
	}*/	
	
	//fade to target Luminance
	if (mode==0) {
	
		if (fadeAlgorithm==0) {
			fadeDirectly(targetBrightness);			
		} else if (fadeAlgorithm==1) {
			fadeSuccessively(fadeDelayUs, targetBrightness);
		} else if (fadeAlgorithm==2) {
			fadeSimultaneous(fadeDelayUs, targetBrightness);
		}
		//cout << "leds stay on for 2s..." << endl;
		//gpioDelay(2000000);
	}
	
	
	
	if (mode==1) {
		uint16_t currentBrightnessSingle = 0;
		cout << "start continuos fade. exit with ctrl+c" << endl;
		while(1) {
				while(currentBrightnessSingle < realPWMrange) {
					currentBrightnessSingle++;
					gpioPWM(pin, currentBrightnessSingle);
					gpioDelay(fadeDelayUs);
					//cout << currentBrightnessSingle << endl;
				}
				while(currentBrightnessSingle > 0) {
					currentBrightnessSingle--;
					gpioPWM(pin, currentBrightnessSingle);
					gpioDelay(fadeDelayUs);
					//cout << currentBrightnessSingle << endl;
				}
			}
	}
	
		
	
	
	
	//}
		
	
	
	//Write brightness to file
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
		
		
	/*cout << "end of program. turn all leds off" << endl; // prints !!!Hello World!!!
	for (int color = 0; color < COLORS; color++) {
		gpioWrite(pins[color], 0);
	}*/
	
	
	//gpioTerminate();
	//cout << "gpio closed" << endl;
	return 0;
}



//*********************************general INIT*******************************************
bool initGeneral(void) {
	#ifdef ONE_PIN_INTERACTIVE
		cout << "enter pin (R/G/B/W || 17/18/22/25(or 27) :" << endl;
		cin >> pin;
		cout << "choosen pin: " << pin << endl;
	#endif
	
	if (gpioInitialise() < 0)
	{
	   cout << "pigpio initialisation failed." << endl;
	   return 0;
	}
	else
	{
	   cout << "pigpio initialised okay." << endl;
	   return 1;
	}
}


//***************************PINS INIT******************************************************


bool initPin(int pin, uint16_t mode) {
	#if PWM_CONFIG
		int realRange = 0;
		int setRange = 0;
		int targetRange = 0;
		
		int targetFrequency = 0;
		int frequency = 0;
	
		cout << "enter target frequency :" << endl;
		cin >> targetFrequency;
		cout << "set frequency to: " << targetFrequency << endl;
		frequency = gpioSetPWMfrequency(pin, targetFrequency);
		cout << "range was set to closest frequency: " << frequency << endl;
		cout << "check frequency: " << gpioGetPWMfrequency(pin) << endl;	
		
		cout << "enter target range :" << endl;
		cin >> targetRange;
		cout << "set range to: " << targetRange << endl;
		realRange = gpioSetPWMrange(pin, targetRange);
		cout << "range was set to real range: " << realRange << endl;
		cout << "check range: " << gpioGetPWMrange(pin) << endl;
	#else
		#ifdef ONE_PIN_INTERACTIVE
		gpioSetPWMfrequency(pin, PWM_FREQUENCY);
		cout << "check frequency: " << gpioGetPWMfrequency(pin) << endl;
		gpioSetPWMrange(pin, PWM_RANGE);
		#endif
		//for all pins now done after defining as an output
	#endif		
	
	
	
	//cout << "enter mode (0 = targetBrightness[0] ; 1 = continiuos fade) :" << endl;
	//cin >> mode;
	cout << "chosen mode: " << mode << endl;
	
	if (mode==0) {
	#ifdef ONE_PIN_INTERACTIVE
		cout << "enter targetBrightness[0] (0 to " << PWM_RANGE << endl;
		cin >> targetBrightness[0];
		cout << "chosen targetBrightness[0]: " << targetBrightness[0] << endl;
	#endif	
	} else if (mode == 1) {
		cout << "enter fadeDelay (in us). default: " << FADE_DELAY_US << endl;
		cin >> fadeDelayUs;
		cout << "chosen fadeDelayUs: " << fadeDelayUs << endl;
	} else {
		cout << "choose correct mode!" << endl; // prints !!!Hello World!!!
		return 0;
	}
	
	if (gpioSetMode(pin, 1)) {
		cout << "error while setting " << pin << " to output" << endl;
		return 0;
	}
	cout << "set " << pin << " to output" << endl;
	
	gpioSetPWMfrequency(pin, PWM_FREQUENCY);
	cout << "check frequency: " << gpioGetPWMfrequency(pin) << endl;
	gpioSetPWMrange(pin, PWM_RANGE);
	realPWMrange = gpioGetPWMrealRange(pin);
	cout << "check real pwm range: " << realPWMrange << endl;
	PWMrange = gpioGetPWMrange(pin);
	cout << "check pwm range: " << PWMrange << endl;
		
	#ifdef ONE_PIN_INTERACTIVE
	//if (!gpioSetMode(pin, 1)) {
	//	cout << "set " << pin << " to output" << endl;
	//}			
	/*int realPWMrange = gpioSetPWMrange(pin, PWM_RANGE);
	cout << "pwm range set to: "  << endl;
	cout << realPWMrange  << endl;*/
	
	if (!gpioPWM(pin, 0)) {
		cout << "pwm set to 0 correctly" << endl;
	}
	#endif
	return 1;
}



//**************************************FADE********************
void fadeSuccessively(uint16_t delay, uint16_t targetBrightness[]) {
	cout << "fading leds successively..." << endl;
	//uint16_t currentBrightness[4] = {0,0,0,0}; 
	for (int color = 0; color < COLORS; color++) {
		//if (currentBrightness[color] < targetBrightness [color]) {
		for (currentBrightness[color]; currentBrightness[color] < targetBrightness[color]; currentBrightness[color]++) {
			gpioPWM(pins[color], currentBrightness[color]);
			gpioDelay(delay);
		}
		//} else if (currentBrightness[color] > targetBrightness [color])	{
		for (currentBrightness[color]; currentBrightness[color] > targetBrightness[color]; currentBrightness[color]--) {
			gpioPWM(pins[color], currentBrightness[color]);
			gpioDelay(delay);
		}
		//}
		gpioPWM(pins[color], targetBrightness[color]);
	}
}

void fadeDirectly(uint16_t targetBrightness[]) {
	for (int color = 0; color < COLORS; color++) {
				//cout << "chosen targetBrightness[0]: " << targetBrightness[0] << endl;		
				gpioPWM(pins[color], targetBrightness[color]);
				cout << "set pin: " << pins[color] << " to chosen brightness (" << targetBrightness[color] << "). Ctrl+C to exit" << endl;
			}
}


void fadeSimultaneous(uint16_t delay, uint16_t targetBrightness[])
{
	cout << "fading leds simultaneous..." << endl;
	for (int step = 0; step < realPWMrange; step++) {
		for (int color = 0; color < COLORS; color++) {
			if (currentBrightness[color] < targetBrightness [color]) {
				currentBrightness[color]++;
				//cout << pins[color] << " " << currentBrightness[color] << endl;
				gpioPWM(pins[color], currentBrightness[color]);
			} else if (currentBrightness[color] > targetBrightness [color])	{
				currentBrightness[color]--;
				//cout << pins[color] << " " << currentBrightness[color] << endl;
				gpioPWM(pins[color], currentBrightness[color]);
			}
		}
		gpioDelay(delay*SIMULTANEOUS_DELAY_FACTOR);
	}
	//fadeDirectly(targetBrightness); //to make sure everything is on the right brightness
}


//**************************************NOT USED******************************************


//in the future for each individual pin
bool initPins(uint16_t mode) {
	#if PWM_CONFIG
		int realRange = 0;
		int setRange = 0;
		int targetRange = 0;
		
		int targetFrequency = 0;
		int frequency = 0;
	
		cout << "enter target frequency :" << endl;
		cin >> targetFrequency;
		cout << "set frequency to: " << targetFrequency << endl;
		frequency = gpioSetPWMfrequency(pin, targetFrequency);
		cout << "range was set to closest frequency: " << frequency << endl;
		cout << "check frequency: " << gpioGetPWMfrequency(pin) << endl;	
		
		cout << "enter target range :" << endl;
		cin >> targetRange;
		cout << "set range to: " << targetRange << endl;
		realRange = gpioSetPWMrange(pin, targetRange);
		cout << "range was set to real range: " << realRange << endl;
		cout << "check range: " << gpioGetPWMrange(pin) << endl;
	#else
		#ifdef ONE_PIN_INTERACTIVE
		gpioSetPWMfrequency(pin, PWM_FREQUENCY);
		cout << "check frequency: " << gpioGetPWMfrequency(pin) << endl;
		gpioSetPWMrange(pin, PWM_RANGE);
		#endif
		//for all pins now done after defining as an output
	#endif		
	
	
	
	//cout << "enter mode (0 = targetBrightness[0] ; 1 = continiuos fade) :" << endl;
	//cin >> mode;
	cout << "chosen mode: " << mode << endl;
	
	if (mode==0) {
	#ifdef ONE_PIN_INTERACTIVE
		cout << "enter targetBrightness[0] (0 to " << PWM_RANGE << endl;
		cin >> targetBrightness[0];
		cout << "chosen targetBrightness[0]: " << targetBrightness[0] << endl;
	#endif	
	} else if (mode == 1) {
		cout << "enter fadeDelay (in us). default: " << FADE_DELAY_US << endl;
		cin >> fadeDelayUs;
		cout << "chosen fadeDelayUs: " << fadeDelayUs << endl;
	} else {
		cout << "choose correct mode!" << endl; // prints !!!Hello World!!!
		return 0;
	}
	
	for (int color = 0; color < COLORS; color++) {
		if (!gpioSetMode(pins[color], 1)) {
			cout << "set " << pins[color] << " to output" << endl;
		}
		gpioSetPWMfrequency(pins[color], PWM_FREQUENCY);
		cout << "check frequency: " << gpioGetPWMfrequency(pins[color]) << endl;
		gpioSetPWMrange(pins[color], PWM_RANGE);
		realPWMrange = gpioGetPWMrealRange(pins[color]);
		cout << "check real pwm range: " << realPWMrange << endl;
		PWMrange = gpioGetPWMrange(pins[color]);
		cout << "check pwm range: " << PWMrange << endl;
	}
	
	
	
	#ifdef ONE_PIN_INTERACTIVE
	if (!gpioSetMode(pin, 1)) {
		cout << "set " << pin << " to output" << endl;
	}			
	/*int realPWMrange = gpioSetPWMrange(pin, PWM_RANGE);
	cout << "pwm range set to: "  << endl;
	cout << realPWMrange  << endl;*/
	
	if (!gpioPWM(pin, 0)) {
		cout << "pwm set to 0 correctly" << endl;
	}
	#endif
	return 1;
}


