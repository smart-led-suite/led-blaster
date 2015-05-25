#include <string.h>
#include <iostream>
#include <stdio.h>
using namespace std;

int main(int argc, char ** argv)
{
   	 char str [80];
  char variable[5];
  char modeName[] = "mode";
  char redb[] = "redb";
  char grnb[] = "grnb";
  char blub[] = "blub";
  char whtb[] = "whtb";
  char exit[] = "exit";
  char waitName[] = "wait";
  bool wait;
  int value;
  int brightness;
  int mode;
  int i;
 // char* pointer = NULL,  nl;
  int scanSuccess = 0;
  
  /*char *lineptr = NULL, nl;
  int n, servo;
  float value;*/

//int mode = 0;
int targetBrightness[4];
printf ("Welcome to your LED fade program \n");
printf ("enter exit = 1 to exit \n");
printf ("enter redb / grnb / blub / whtb = value to set brightness for a specific color \n");
printf ("Enter mode = 0/1 to set mode \n");
printf ("Enter wait = 0/1 (1 to wait until every color has been updated, then fade simutaneous \n");

while(true) {
  printf ("Enter your configuration: ");
  scanSuccess = scanf ("%s = %d",&variable, &value);  
  printf("variable changed: %s set to %d. \n", variable, value);
  printf("return value: %d \n", scanSuccess);
  cout << variable << endl;
 // cout << exit << endl;
  if (strcmp(exit, variable)==0) {
  	printf("exit program. thank you. \n");
  	return 0;
  } else if (strcmp(modeName, variable)==0) {
  	mode = value;
  } else if (strcmp(waitName, variable)==0) {
  	wait = value;
  } else if (strcmp(whtb, variable)==0) {
  	targetBrightness[0] = value;
  } else if (strcmp(redb, variable)==0) {
  	targetBrightness[1] = value;
  } else if (strcmp(grnb, variable)==0) {
  	targetBrightness[2] = value;
  } else if (strcmp(blub, variable)==0) {
  	targetBrightness[3] = value;
  }
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
 }	
  
  
  
  
   
   	/*
	//OPEN CONFIG FILE IN OUR APPLICAITONS DIRECTORY OR CREATE IT IF IT DOESN'T EXIST
	FILE *file1;
	unsigned char file_data[100];
	const char *filename1 = "currentBrightness.led";

	file1 = fopen(filename1, "rb");
	if (file1)
	{
		//----- FILE EXISTS -----
		fread(&file_data[0], sizeof(unsigned char), 100, file1);

		printf("File opened, some byte values: %i %i %i %i\n", file_data[0], file_data[1], file_data[2], file_data[3]);

		fclose(file1);
	}
	else
	{
		//----- FILE NOT FOUND -----
		printf("File not found\n");

		//Write new file
		file1 = fopen(filename1, "wb");
		if (file1)
		{
			printf("Writing new file\n");
			file_data[0] = 10;
			file_data[1] = 11;
			file_data[2] = 12;
			file_data[3] = 13;

			fwrite(&file_data[0], sizeof(unsigned char), 100, file1) ;

			fclose(file1);
		}
	}	
}*/






/* //#include <istringstream>
//#include <iostream>
#include <string>       // std::string
#include <iostream>     // std::cout
#include <sstream>      // std::istringstream

using namespace std;

const char config[] = "url=http://mysite.com\n"
                      "file=main.exe\n"
                      "true=0";

int main() {

std::istringstream is_file(config);

std::string line;
while( std::getline(is_file, line) )
{
  std::istringstream is_line(line);
  std::string key;
  if( std::getline(is_line, key, '=') )
  {
    std::string value;
    if( std::getline(is_line, value) ) 
      std::store_line(key, value);
  }
}

}*/
