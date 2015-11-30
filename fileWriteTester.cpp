// basic file operations
#include <iostream>
#include <fstream>
using namespace std;

int main () {
  cout << "hello" << endl;
  ofstream myfile;
  myfile.open ("/var/www/html/example.csv");
  if (myfile.is_open()) 
  {
   	/* ok, proceed with output */ 
   	//myfile << "Writing this to a file.\n";
   	myfile <<  "w" << ";";
	myfile << "25" << ";";
	myfile << "200" <<"\n";
   	cout << "successfully written to file" << endl;
   }
   else
   {
   	cout << "file couldnt be opened" << endl;
   }		
  myfile.close();
  return 0;
}
