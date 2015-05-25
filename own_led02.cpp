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
	
	const char *currentBrightnessFilename = "currentBrightness.led";

	cbFile = fopen(currentBrightnessFilename, "rb");
	if (cbFile)
	{
		//----- FILE EXISTS -----
		fread(&currentBrightness[0], sizeof(unsigned char), 100, cbFile);

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
	uint16_t targetBrightness[4];
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
	
		

	
	//fade to target Luminance
	if (mode==0) {
	
		if (fadeAlgorithm==0) {
			fadeDirectly(targetBrightness);			
		} else if (fadeAlgorithm==1) {
			fadeSuccessively(fadeDelayUs, targetBrightness);
		}
		cout << "leds stay on for 2s..." << endl;
		gpioDelay(2000000);
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
	}
	
	
	gpioTerminate();
	cout << "gpio closed" << endl;*/
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


