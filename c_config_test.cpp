#include <stdio.h>
#include <iostream>
#include <stdio.h>
using namespace std;

int main(int argc, char ** argv)
{
   	 char str [80];
  char variable[4];
  int brightness;
  int mode;
  int i;
 // char* pointer = NULL,  nl;
  int scanSuccess = 0;
  
  char *lineptr = NULL, nl;
  int n, servo;
  float value;






  printf ("Enter your configuration: ");
  scanSuccess = scanf ("%s = %d",&variable, &brightness);  
  printf("variable changed: %s set to %d. \n", variable, brightness);
  printf("return value: %cd \n", scanSuccess);
  printf("sizeof %d", sizeof(variable));
  for (int i=0; i < sizeof(variable); i++) {
  	printf("value of variable[%d] is %s", i, variable[i]);
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
