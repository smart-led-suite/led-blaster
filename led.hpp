#ifndef _led_h
#define _led_h
#include <stdint.h> //libary which includes uint8_t etc.
#include <string>
//compiler params:
// g++ -std=c++0x -o object led.hpp led.cpp init.cpp -lpigpio -lrt -lpthread

class LED
{
  public:
    //constructor
    LED(std::string led_colorcode, uint16_t led_pin, bool led_isColor, uint16_t led_currentBrightness, uint16_t led_argetBrightness);
    //getter
    std::string getColorCode();
    uint16_t getPin();
    bool getIsColor();
    uint16_t getCurrentBrightness();
    uint16_t getTargetBrightness();
    int getPwmSteps();
    //setter
    void setColorCode(std::string new_colorcode);
    void setPin(uint16_t newpin);
    void setIsColor(bool newisColor);
    void setCurrentBrightness(uint16_t new_cBrightness);
    void setTargetBrightness(uint16_t new_tBrightness);
    //functions
    void writeBrightnessToPin(uint16_t brightness);

  private:
    int initPin(void);
    std::string colorcode;
    uint16_t pin;
    bool isColor;
    uint16_t currentBrightness;
    uint16_t targetBrightness;
    int pwmSteps;
};
#endif //EOF
