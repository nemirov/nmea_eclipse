//============================================================================
// Name        : nmea.cpp
// Author      : crir
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <fstream>
#include "nmeaparser.h"

#include <unistd.h> //sleep()
using namespace std;

int main() {
	string file = "/Users/c/Documents/workspace/nmea/Debug/nmea.test";
	ifstream ifs (file.c_str());
	char string_nmea[1024];

	NmeaParser *parser = new NmeaParser();
	//string tmp = "$GPGGA,111609.14,5001.27,N,3613.06,E,3,08,0.0,10.2,M,0.0,M,,0000*70";

	if(ifs){
		while(!ifs.eof()){
			ifs.getline(string_nmea, sizeof(string_nmea));
			string str = (const char*) string_nmea;
			parser->Parse(&str);
			sleep (1);
		}
	} else {
		cout << "Error open file" << endl;
	}



	cout << "Hello World" << endl; // prints Hello World
	return 0;
}
