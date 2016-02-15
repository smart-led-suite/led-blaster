#include "led-blaster-pre.hpp"
#include "led.hpp"

/* ----------------------------------------------------------------------------
   libconfig - A library for processing structured configuration files
   Copyright (C) 2005-2010  Mark A Lindner
   This file is part of libconfig.
   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public License
   as published by the Free Software Foundation; either version 2.1 of
   the License, or (at your option) any later version.
   This library is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.
   You should have received a copy of the GNU Library General Public
   License along with this library; if not, see
   <http://www.gnu.org/licenses/>.
   ----------------------------------------------------------------------------
*/

// g++ -std=c++0x  -o example configExample.cpp -lconfig++

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <libconfig.h++>

using namespace std;
using namespace libconfig;

// This example reads the configuration file 'example.cfg' and displays
// some of its contents.
//own private function so we have the config values at the top
bool assignConfigValues (std::string key, std::string value)
{
  if (key.compare("time") == 0) {
    fadeTimeMs = std::stoi(value);
    return 0;
  }
  if (key.compare("server_path") == 0)
  {
    LED_WEB_SERVER_PATH = value;
    return 0;
  }
  return 1;
}


int readConfig (void)
{
  Config cfg;

  // Read the file. If there is an error, report it and exit.
  try
  {
    cfg.readFile("example.cfg");
  }
  catch(const FileIOException &fioex)
  {
    std::cerr << "I/O error while reading file." << std::endl;
    return(EXIT_FAILURE);
  }
  catch(const ParseException &pex)
  {
    std::cerr << "Parse error at " << pex.getFile() << ":" << pex.getLine()
              << " - " << pex.getError() << std::endl;
    return(EXIT_FAILURE);
  }

  // Get the store name.
  try
  {
    int fadeTimeMs = cfg.lookup("time");
    cout << "time: " << fadeTimeMs << endl << endl;
  }
  catch(const SettingNotFoundException &nfex)
  {
    cerr << "No 'time' setting in configuration file." << endl;
  }
  // Get the store name.
  try
  {
    string LED_WEB_SERVER_PATH = cfg.lookup("server_path");
    cout << "serverPath: " << LED_WEB_SERVER_PATH << endl << endl;
  }
  catch(const SettingNotFoundException &nfex)
  {
    cerr << "No 'server_path' setting in configuration file." << endl;
  }

  return(EXIT_SUCCESS);
}

//old functions:



/*
//read general config from config file
void readConfig(void)
{
  const char *configFileName = "/etc/led-blaster.cfg";
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
          if (assignConfigValues(key, value))
          {
            std::cerr << "configFile read error at" << line << std::endl;
          }
      }
    }
    #ifdef DEBUG
    std::cout << "fadetime: " << fadeTimeMs << std::endl;
  //  std::cout << "server: " << LED_WEB_SERVER_PATH << std::endl;
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
       fadeTimeMs = FADE_TIME_MS;

   }
}
*/
