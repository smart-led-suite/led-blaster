//============================================================================
// Name        : hello.cpp
// Author      : Sandesh
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
// ToDo	       : 
//	(done)	 - new universal initPin
//	(done)	 - introduce header file so the functions can be placed below the main part
//	(done)	 - introduce file to save current luminances
//	(done)	 - introduce simultaneous fadeAlgorithm
//		 - improve simultaneous algorithm
//		 - move currentBrightness read/write to own function
//		 - introduce other fadeModes (aka exp fade)
//		 - introduce pipes (major)
//		 - introduce some other modes (fade w/ overlap of different colors)
//		 - introduce music mode
//		 - introduce config file w/ led pins etc. 
//============================================================================

#include </var/www-coming-soon/led-blaster-pre.hpp>
using namespace std;


//pins		w, r, g, b
int pins[4] = {25, 17,18,22};
uint16_t targetBrightness[4];
int fadeAlgorithm = 0;
int realPWMrange = 0;
int PWMrange = 0;
int fadeDelayUs = FADE_DELAY_US;
int pin = 17; //pin used for continious fade on 1 pin
uint16_t mode = 0; // mode is now a global variable! set mode to 0 (default)
bool threadWasCreated = false; //default is: no thread (obviusly)
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
	readCurrentBrightness();
	
	
	
	
	
	
	//init pwm
	if(initGeneral()) {				//initializes the pigpio libary. returns 0 if there was no problem
		cout << "initGeneral failed!" << endl;	//if there's a problem print it out and exit the program
		return 1;
	}
	
	
	//initialize all pins
	for (int color = 0; color < COLORS; color++) {
		if(initPin(pins[color], mode) != 0) {  				//initializes each pin. returns 0 if everything went ok
			cout << "error in initPin " << pins[color] << endl;	//print that there has been an error if this happens (very unlikely)
			return 2;						//and exit the program
		}	
	}
	
	//init variables to use with the interactive live input
	char variable[5];
	//command names, its only possible to compare char arrays
	char mode_cmd[] = "mode";
	char wait_cmd[] = "wait";
  	char redb_cmd[] = "redb";
  	char grnb_cmd[] = "grnb";
  	char blub_cmd[] = "blub";
  	char whtb_cmd[] = "whtb";
  	char exit_cmd[] = "exit";
  	
  	
  	//values
  	uint16_t wait=0;
  	uint16_t value = 0;
  	uint16_t brightness;
  	uint16_t waitCounter = 0; //used only in live mode. 
  	uint16_t waitCounterW = 0;
  	uint16_t waitCounterR= 0;
  	uint16_t waitCounterG = 0;
  	uint16_t waitCounterB = 0;
 	// char* pointer = NULL,  nl;
  	int scanSuccess = 0;
  	
  	/*char *lineptr = NULL, nl;
  	int n, servo;
  	float value;*/
	
//	int mode = 0;
	//int targetBrightness[4];
	#ifdef CLI_FADE
	printf ("Welcome to your LED fade program \n");
	printf ("enter exit = 1 to exit \n");
	printf ("enter redb / grnb / blub / whtb = value to set brightness for a specific color \n");
	printf ("Enter mode = 0/1 to set mode \n");
	printf ("Enter wait = 0/1 (1 to wait until every color has been updated, then fade simutaneous \n");
	#endif
	
	const char *configFilename = "inputData.txt";
	FILE *confFile;
	
	char fileInputString[5] = "    ";
	char fileInputValue[20] = "                   ";
	
	

while(true) {
	
	  	cout << "(loop start) Enter your configuration:  ";
	  	scanSuccess = scanf ("%s = %d",&variable, &value);  
	  	printf("variable changed: %s set to %d. \n", variable, value);
	  	printf("return value: %d \n", scanSuccess);
	  	if (strcmp(exit_cmd, variable)==0) {
	  		printf("exit program. thank you. \n");
			writeCurrentBrightness();
			gpioTerminate();
			return 0;
	  	} else if (strcmp(mode_cmd, variable)==0) {
	  		mode = value;
	  	} else if (strcmp(wait_cmd, variable)==0) {
	  		wait = value;
	  		waitCounter = value;
	  	} else if (strcmp(whtb_cmd, variable)==0) {
	  		targetBrightness[0] = value;
	  		if (wait) {
		  			waitCounter--;
		  		}	
	  	} else if (strcmp(redb_cmd, variable)==0) {
	  		targetBrightness[1] = value;
	  		if (wait) {
		  			waitCounter--;
		  		}
	  	} else if (strcmp(grnb_cmd, variable)==0) {
	  		targetBrightness[2] = value;
	  		if (wait) {
		  			waitCounter--;
		  		}
	  	} else if (strcmp(blub_cmd, variable)==0) {
	  		targetBrightness[3] = value;
	  		if (wait) {
		  			waitCounter--;
		  		}	
	  	}
		  	
	  	if (mode == 0) {
	  		/* if (threadWasCreated)
	  			pthread_exit(NULL); */
	  		if (wait == 0 || waitCounter == 0) {
		  		fadeSimultaneous(fadeDelayUs, targetBrightness);
		  	}
		  	cout << "waitCounter: " << waitCounter << endl;
			cout << "mode: " << mode << endl;
			cout << "wait: " << wait << endl;
			cout << "targetBrightness w " << targetBrightness[0] << endl;
			cout << "targetBrightness r " <<  targetBrightness[1] << endl;
			cout << "targetBrightness g " <<  targetBrightness[2] << endl;
			cout << "targetBrightness b " <<  targetBrightness[3] << endl;
		  }
	  	
	  	
	  	
	  	if (mode==1) {
	  		
	  		pthread_t continiousFadeThread;
	  		void *randomPointer;
	  		//void (*modeContFadePointer)(void);
	  		//modeContFadePointer = modeContiniousFade;
	  		int a = pthread_create(&continiousFadeThread, NULL, modeContiniousFade , NULL);
	  		if (a) {
	  			cout << "Something happened while starting the modeContiniousFade Thread" << endl;
	  			cout << "error no: " << a << endl;
	  			}
	  			
	  		threadWasCreated = true;
	  		//modeContiniousFade();
			
		}	
			
	}
	pthread_exit(NULL);	
}


//***************************************FUNCTIONS***************************************

//MODES

//continious fade (mode = 1)
// this function is initialized w/ a pointer in order to create a separate thread in main()
void *modeContiniousFade(void*) {
	uint16_t currentBrightnessSingle = 0;
		cout << "continious fade mode. set pin: 17/18/22/25 R/G/B/W: " << endl;
		int pin = 25;
		//cin >> pin;
		cout << "value of mode is: " << mode << endl;
		cout << "start continuos fade on pin " << pin << ". exit with mode = 0" << endl;
		cout << "Enter your configuration: " << endl ; //surprisingly, without endl it prints nothing (at the begin of the main loop the same command works)
		while(mode == 1) {
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
		mode = 0;
		pthread_exit(NULL);
		
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
	   return 1;
	}
	else
	{
	   cout << "pigpio initialised okay." << endl;
	   return 0;
	}
}


//***************************PINS INIT******************************************************


bool initPin(int pin, uint16_t mode) {
	#if PWM_CONFIG
		int realRange = 0;
		int setRange = 0;
		int targetRange = 0;
		
	test	int targetFrequency = 0;
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
	
	
	if (gpioSetMode(pin, 1)) {
		cout << "error while setting " << pin << " to output" << endl;
		return 1;
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
	return 0;
}

//*************** R/W CURRENT BRIGHTNESS ************************


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
