// Copyright 2005, Google Inc.
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//     * Neither the name of Google Inc. nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

// A sample program demonstrating using Google C++ testing framework.
//
// Author: wan@google.com (Zhanyong Wan)


// This sample shows how to write a simple unit test for a function,
// using Google C++ testing framework.
//
// Writing a unit test using Google C++ testing framework is easy as 1-2-3:


// Step 1. Include necessary header files such that the stuff your
// test logic needs is declared.
//
// Don't forget gtest.h, which declares the testing framework.

#include <iostream>
#include <cstdlib>
#include <pigpio.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <map>
#include <vector>

#include <string>
#include <pthread.h>
#include <stdint.h> //libary which includes uint8_t etc.
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/stat.h>
#include <unistd.h>
#include <linux/stat.h>

#include <limits.h>
#include "gtest/gtest.h"

#include "pigpio.h"
#include "../led.hpp"

//#include "../file.hpp"
#include "../init.hpp"
#include "../config.h"
/*
#include "../modes.hpp"
#include "../fade.hpp"
#include "../config.h"


#include "../init.hpp"

#include "../fifo.hpp"
*/
#include "../led-blaster-pre.hpp"

// Step 2. Use the TEST macro to define your tests.
//
// TEST has two parameters: the test case name and the test name.
// After using the macro, you should define your test logic between a
// pair of braces.  You can use a bunch of macros to indicate the
// success or failure of a test.  EXPECT_TRUE and EXPECT_EQ are
// examples of such macros.  For a complete list, see gtest.h.
//
// <TechnicalDetails>
//
// In Google Test, tests are grouped into test cases.  This is how we
// keep test code organized.  You should put logically related tests
// into the same test case.
//
// The test case name and the test name should both be valid C++
// identifiers.  And you should not use underscore (_) in the names.
//
// Google Test guarantees that each test you define is run exactly
// once, but it makes no guarantee on the order the tests are
// executed.  Therefore, you should write your tests in such a way
// that their results don't depend on their order.
//
// </TechnicalDetails>


struct ledClassConstructorTest : testing::Test
 {

  ledClassConstructorTest()
  {
    //EXPECT_EQ(0, initGeneral());

    if(initGeneral())
    {
      std::cout << "usage of pigpio is not possible. the led_test cannot be executed" << std::endl;
      exit(0);
    }
  }
  ~ledClassConstructorTest()
  {
    //mode = 0; //so we won't have any problems with threads and so on
    //printf("\nUser pressed Ctrl+C || SIGINT detected. Turn LEDs off.\n");
    // we want to turn all GPIOs of to avoid some strange stuff
    //writeCurrentBrightness(); //useless but we'll save it anyway
    //printf("terminate gpio \n");
    gpioTerminate(); //terminates GPIO (but doesn't necessarily turn all gpios off
  }
};

struct ledTest : testing::Test
 {
  LED * led[4] = {NULL, NULL, NULL, NULL};
  ledTest()
  {
    if(initGeneral())
    {
      std::cout << "usage of pigpio is not possible. the led_test cannot be executed" << std::endl;
      exit(0);
    }
    led[0] = new LED("b", 25, true, 0, 0);
    led[1] = new LED("w", 17, false, 0, 0);
    led[2] = new LED("r", 18, true, 0, 0);
    led[3] = new LED("g", 22, true, 0, 0);
  }
  ~ledTest()
  {
    //delete[] led;
    gpioTerminate();
  }
};


TEST_F(ledClassConstructorTest, ledConstructor)
{
  LED ledObject("w", 25, 0, 0, 0);
  EXPECT_EQ(25,ledObject.getPin());
  EXPECT_EQ("w",ledObject.getColorCode());
  EXPECT_EQ(false, ledObject.IsColor());
  EXPECT_EQ(0, ledObject.getCurrentBrightness());
  EXPECT_EQ(0, ledObject.getTargetBrightness());
  EXPECT_EQ(PWM_RANGE, ledObject.getPwmSteps());
}

TEST_F(ledTest, setTargetBrightnessTest)
{
  for (size_t ledNumber = 0; ledNumber < 1; ledNumber++)
  {
    //set targetBrightness to a valid value ( the maximum possible value)
    led[ledNumber]->setTargetBrightness( led[ledNumber]->getPwmSteps() );
    EXPECT_EQ(led[ledNumber]->getPwmSteps(), led[ledNumber]->getTargetBrightness()) << "normal assignment doesn't work";
    //set it to a negative value
    led[ledNumber]->setTargetBrightness( -100 );
    EXPECT_EQ(0, led[ledNumber]->getTargetBrightness()) << "negative values produce an error";
    //set it to a too big number
    led[ledNumber]->setTargetBrightness( led[ledNumber]->getPwmSteps() + 1);
    EXPECT_EQ(led[ledNumber]->getPwmSteps(), led[ledNumber]->getTargetBrightness()) << "values which are too big produce an error";
  }

}

TEST_F(ledTest, setCurrentBrightnessTest)
{
  for (size_t ledNumber = 0; ledNumber < 4; ledNumber++)
  {
    //set CurrentBrightness to a valid value ( the maximum possible value)
    led[ledNumber]->setCurrentBrightness( led[ledNumber]->getPwmSteps() );
    EXPECT_EQ(led[ledNumber]->getPwmSteps(), led[ledNumber]->getCurrentBrightness()) << "normal assignment doesn't work";
    //set it to a negative value
    led[ledNumber]->setCurrentBrightness( -100 );
    EXPECT_EQ(0, led[ledNumber]->getCurrentBrightness()) << "negative values produce an error";
    //set it to a too big number
    led[ledNumber]->setCurrentBrightness( led[ledNumber]->getPwmSteps() + 1);
    EXPECT_EQ(led[ledNumber]->getPwmSteps(), led[ledNumber]->getCurrentBrightness()) << "values which are too big produce an error";
    led[ledNumber]->setCurrentBrightness(0);
  }

}

TEST_F(ledTest, fadeFunctionTest)
{
  for (size_t ledNumber = 0; ledNumber < 4; ledNumber++)
  {
    int fadeTime = 400;

    led[ledNumber]->setTargetBrightness(0);
    led[ledNumber]->fade((void *)&fadeTime);
    EXPECT_EQ(led[ledNumber]->getCurrentBrightness(), led[ledNumber]->getTargetBrightness());
    led[ledNumber]->setTargetBrightness(led[ledNumber]->getPwmSteps());
    led[ledNumber]->fade((void *)&fadeTime);
    EXPECT_EQ(led[ledNumber]->getCurrentBrightness(), led[ledNumber]->getTargetBrightness());
    led[ledNumber]->setTargetBrightness(0);
    led[ledNumber]->fade((void *)&fadeTime);
    EXPECT_EQ(led[ledNumber]->getCurrentBrightness(), led[ledNumber]->getTargetBrightness());
  }
}

TEST_F(ledTest, fadeThreadsOneThreadTest)
{
  //int fadeTime = 1000;
    led[0]->setCurrentBrightness(1000);
    led[0]->setTargetBrightness(0);
    led[0]->fadeInThread(1000);
  //  void ** p = NULL;
    //pthread_join(  led[0]->getFadeThread(), NULL);
    //EXPECT_EQ(led[0]->getCurrentBrightness(), led[0]->getTargetBrightness());
    //usleep(1000);
  /*  led[ledNumber]->setTargetBrightness(led[ledNumber]->getPwmSteps());
    led[ledNumber]->fadeInThread(200);
    EXPECT_EQ(true, led[ledNumber]->isFading());
    led[ledNumber]->setTargetBrightness(0);
    led[ledNumber]->fadeInThread(fadeTime);
    EXPECT_EQ(led[ledNumber]->getCurrentBrightness(), led[ledNumber]->getTargetBrightness());*/
//  }
}
TEST_F(ledTest, fadeThreadsTest)
{

  /*  int fadeTime = 1000;
  for (size_t ledNumber = 0; ledNumber < 4; ledNumber++)
  {
    led[ledNumber]->setTargetBrightness(0);
    led[ledNumber]->fadeInThread(fadeTime);
  /*  void ** p = NULL;
    pthread_join(  led[ledNumber]->getFadeThread(), p);*/
    //EXPECT_EQ(led[ledNumber]->getCurrentBrightness(), led[ledNumber]->getTargetBrightness());
  /*  led[ledNumber]->setTargetBrightness(led[ledNumber]->getPwmSteps());
    led[ledNumber]->fadeInThread(200);
    EXPECT_EQ(true, led[ledNumber]->isFading());
    led[ledNumber]->setTargetBrightness(0);
    led[ledNumber]->fadeInThread(fadeTime);
    EXPECT_EQ(led[ledNumber]->getCurrentBrightness(), led[ledNumber]->getTargetBrightness());
  }*/
}
// Step 3. Call RUN_ALL_TESTS() in main().
//
// We do this by linking in src/gtest_main.cc file, which consists of
// a main() function which calls RUN_ALL_TESTS() for us.
//
// This runs all the tests you've defined, prints the result, and
// returns 0 if successful, or 1 otherwise.
//
// Did you notice that we didn't register the tests?  The
// RUN_ALL_TESTS() macro magically knows about all the tests we
// defined.  Isn't this convenient?
