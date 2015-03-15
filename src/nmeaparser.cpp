/*
 * nmea_parser.cpp
 *
 *  Created on: Mar 12, 2015
 *      Author: nemirov
 */

#ifndef NMEAPARSER_CPP_
#define NMEAPARSER_CPP_

// NmeaParser.cpp: implementation of the NmeaParser class.
//
//////////////////////////////////////////////////////////////////////

#include "nmeaparser.h"
#include "common.h"

#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <math.h>
#include <typeinfo>

using namespace std;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

NmeaParser::NmeaParser() {
	nmeaArray = new vector<string>;
}

NmeaParser::~NmeaParser() {

}

void NmeaParser::Parse(string *nmeaString) {

	string gps_cfg_rtrim_chars = "$ \r\t\n";

	nmeaArray->clear();

	gps_lrtrim(nmeaString, &gps_cfg_rtrim_chars);

	if (checkCrc(nmeaString) != -1) {

		if (nmeaArray->at(0).size() != 5) {
			//Not $GXxxx
			return;
		}

		//Sentence Codes
		string sentence_codes = nmeaArray->at(0).substr(2, 3);
		cout << "Sentence codes : " << sentence_codes << endl;
		cout << "Length array : " << nmeaArray->size() << endl;

		if (sentence_codes == string("GGA") && nmeaArray->size() == 15) {

			GGA2Info();

		} else if (sentence_codes == string("RMC") && nmeaArray->size() == 13) {

			RMC2Info();

		}
		/*
		 else if( strcmp(addressField, "GPGSA") == NULL )
		 {
		 ProcessGPGSA(buf, bufSize);
		 }
		 else if( strcmp(addressField, "GPGSV") == NULL )
		 {
		 ProcessGPGSV(buf, bufSize);
		 }
		 else if( strcmp(addressField, "GPRMB") == NULL )
		 {
		 ProcessGPRMB(buf, bufSize);
		 }

		 else if( strcmp(addressField, "GPZDA") == NULL )
		 {
		 ProcessGPZDA(buf, bufSize);
		 }*/

	};
}

/*
 XXGGA Sentence format

 $GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M, ,*47
 |   |	  |			 |			 | |  |	  |		  |      | |
 |   |	  |			 |			 | |  |	  |		  |		 | checksum data
 |   |	  |			 |			 | |  |	  |		  |		 |
 |   |	  |			 |			 | |  |	  |		  |		 empty field
 |   |	  |			 |			 | |  |	  |		  |
 |   |	  |			 |			 | |  |	  |		  46.9,M Height of geoid (m) above WGS84 ellipsoid
 |   |	  |			 |			 | |  |	  |
 |   |	  |			 |			 | |  |	  545.4,M Altitude (m) above mean sea level
 |   |	  |			 |			 | |  |
 |   |	  |			 |			 | |  0.9 Horizontal dilution of position (HDOP)
 |   |	  |			 |			 | |
 |   |	  |			 |			 | 08 Number of satellites being tracked
 |   |	  |			 |			 |
 |   |	  |			 |			 1 Fix quality:	0 = invalid
 |   |	  |			 |							1 = GPS fix (SPS)
 |   |	  |			 |							2 = DGPS fix
 |   |	  |			 |							3 = PPS fix
 |   |	  |			 |							4 = Real Time Kinematic
 |   |	  |			 |							5 = Float RTK
 |   |	  |			 |							6 = estimated (dead reckoning) (2.3 feature)
 |   |	  |			 |							7 = Manual input mode
 |   |	  |			 |							8 = Simulation mode
 |   |	  |			 |
 |   |	  |			 01131.000,E Longitude 11 deg 31.000' E
 |   |	  |
 |   |	  4807.038,N Latitude 48 deg 07.038' N
 |   |
 |   123519 Fix taken at 12:35:19 UTC
 |
 GGA Global Positioning System Fix Data

 */
void NmeaParser::GGA2Info() {

	// Time
	setTime(nmeaArray->at(1));

	//Latitude
	setLatitude(nmeaArray->at(2));

	//Latitutde hemisphere
	setLatHemisphere(nmeaArray->at(3));

	//Longitude
	setLongitude(nmeaArray->at(4));

	//Longitude hemisphere
	setLonHemisphere(nmeaArray->at(5));

	//GPS Quality indicator
	setFix(nmeaArray->at(6).c_str());

	//Satellites are in view
	setInview(nmeaArray->at(7));

	//Horizontal Dilution of Precision (HDOP)
	setHDOP(nmeaArray->at(8).c_str());

	//Altitude
	setElv(nmeaArray->at(9));
	setElvMeter(nmeaArray->at(10));

	//Height of geoid above WGS84 ellipsoid
	setHeightOfGeode(nmeaArray->at(11));
	setHeightMeter(nmeaArray->at(12));

}

void NmeaParser::ProcessGPGSA(const char *buf, const unsigned int bufSize) {

}

void NmeaParser::ProcessGPGSV(const char *buf, const unsigned int bufSize) {

}

void NmeaParser::ProcessGPRMB(const char *buf, const unsigned int bufSize) {

}

/*
 Format

 eg4. for NMEA 0183 version 3.00 active the Mode indicator field is added
 $GPRMC,hhmmss.ss,A,llll.ll,a,yyyyy.yy,a,x.x,x.x,ddmmyy,x.x,a,m*hh
 Field #
 1    = UTC time of fix
 2    = Data status (A=Valid position, V=navigation receiver warning)
 3    = Latitude of fix
 4    = N or S of longitude
 5    = Longitude of fix
 6    = E or W of longitude
 7    = Speed over ground in knots
 8    = Track made good in degrees True
 9    = UTC date of fix
 10   = Magnetic variation degrees (Easterly var. subtracts from true course)
 11   = E or W of magnetic variation
 12   = Mode indicator, (A=Autonomous, D=Differential, E=Estimated, N=Data not valid)
 13   = Checksum

 */
void NmeaParser::RMC2Info() {

	if (nmeaArray->at(2) == string("A")) {
		// Time
		setTime(nmeaArray->at(1));

		//Latitude
		setLatitude(nmeaArray->at(3));

		//Latitutde hemisphere
		setLatHemisphere(nmeaArray->at(4));

		//Longitude
		setLongitude(nmeaArray->at(5));

		//Longitude hemisphere
		setLonHemisphere(nmeaArray->at(6));

		//Speed over ground km/h
		setSpeed(nmeaArray->at(7));

		// Course Made Good, degrees true
		setDirection(nmeaArray->at(8));

		//UTC Date of fix
		setDate(nmeaArray->at(9));

		//Magnetic variation
		setDeclination(nmeaArray->at(10));

	};
}

void NmeaParser::ProcessGPZDA(const char *buf, const unsigned int bufSize) {

}

/*
 * Functions settings
 */
string itoa(int value, int base) {

		string buf;

		// check that the base if valid
		if (base < 2 || base > 16) return buf;

		enum { kMaxDigits = 35 };
		buf.reserve( kMaxDigits ); // Pre-allocate enough space.

		int quotient = value;

		// Translating number to string with base:
		do {
			buf += "0123456789abcdef"[ abs( quotient % base ) ];
			quotient /= base;
		} while ( quotient );

		// Append the negative sign
		if ( value < 0) buf += '-';

		reverse( buf.begin(), buf.end() );
		return buf;
	}

int NmeaParser::checkCrc(string *message) {
	string separator_value = ",";

	int length_sentence = gps_split_param(*message, nmeaArray, &separator_value);

	//int length_end_message = gps_split_param(
	//		&(nmeaArray->at(nmeaArray->size() - 1)), tmp_arr, &separator_crc);

	unsigned int i =0;
	int  calc_crc = 0;

	while (message->at(i) != '*') {
		if (i == message->size()) {
			return -1;
		};
		calc_crc ^= message->at(i);
		i++;
	}

	string crc = message->substr(i+1, i+2);
	transform(crc.begin(), crc.end(), crc.begin(), ::tolower);

	if (crc ==itoa(calc_crc, 16)){
		return -1;
	}







	return 0;

}
void NmeaParser::setNavSystem(string system) {

	string nav_system = system.substr(0, 2);

	if (nav_system == string("GP")) {
		nmeaINFO.nav_system = NMEA_NAV_SYSTEM_GPS;
	} else if (nav_system == string("GL")) {
		nmeaINFO.nav_system = NMEA_NAV_SYSTEM_GLONASS;
	} else if (nav_system == string("GN")) {
		nmeaINFO.nav_system = NMEA_NAV_SYSTEM_GPS_GLONASS;
	} else {
		nmeaINFO.nav_system = NMEA_NAV_SYSTEM_UNKNOWN;
	}

}

void NmeaParser::setDate(string date) {
	if (date.size() > 6) {
		nmeaINFO.utc_day = atoi(date.substr(0, 2).c_str());
		nmeaINFO.utc_mon = atoi(date.substr(2, 2).c_str());
		nmeaINFO.utc_year = atoi(date.substr(4, 2).c_str());
	}
}

void NmeaParser::setTime(string time) {

	if (time.size() > 6) {
		nmeaINFO.utc_hour = atoi(time.substr(0, 2).c_str());
		nmeaINFO.utc_min = atoi(time.substr(2, 2).c_str());
		nmeaINFO.utc_sec = atoi(time.substr(4, 2).c_str());
	}
}

void NmeaParser::setLatitude(string latitude) {
	string separator = ".";
	vector<string> tmp_array;
	if (gps_split_param(latitude, &tmp_array, &separator) != -1) {
		nmeaINFO.lat_deg = atoi(tmp_array.at(0).substr(0, 2).c_str());
		nmeaINFO.lat_min = atoi(tmp_array.at(0).substr(2, 2).c_str());
		nmeaINFO.lat_sec = floor(
				atof((string("0.") + tmp_array.at(1)).c_str()) * 60 + 0.5);

	};
}

void NmeaParser::setLatHemisphere(string hem) {
	nmeaINFO.lat_hemisphere = hem;
}

void NmeaParser::setLongitude(string longitude) {
	string separator = ".";
	vector<string> tmp_array;
	if (gps_split_param(longitude, &tmp_array, &separator) != -1) {
		nmeaINFO.lon_deg = atoi(tmp_array.at(0).substr(0, 3).c_str());
		nmeaINFO.lon_min = atoi(tmp_array.at(0).substr(3, 2).c_str());
		nmeaINFO.lon_sec = floor(
				atof((string("0.") + tmp_array.at(1)).c_str()) * 60 + 0.5);

	};
}

void NmeaParser::setLonHemisphere(string hem) {
	nmeaINFO.lon_hemisphere = hem;
}

void NmeaParser::setFix(string fix) {
	nmeaINFO.fix = atoi(fix.c_str());
}

void NmeaParser::setInview(string inview) {
	nmeaINFO.inview = atoi(inview.c_str());
}

void NmeaParser::setHDOP(string hdop) {
	nmeaINFO.HDOP = atof(hdop.c_str());
}

void NmeaParser::setElv(string elv) {
	nmeaINFO.elv = atof(elv.c_str());
}

void NmeaParser::setElvMeter(string meter) {
	nmeaINFO.elv_meter = meter;
}

void NmeaParser::setHeightOfGeode(string height) {
	nmeaINFO.height_of_geode = atof(height.c_str());
}

void NmeaParser::setHeightMeter(string meter) {
	nmeaINFO.geode_meter = meter;
}

void NmeaParser::setSpeed(string speed) {
	nmeaINFO.speed = atof(speed.c_str()) * 1.852;
}

void NmeaParser::setDirection(string direction) {
	nmeaINFO.direction = atof(direction.c_str());
}

void NmeaParser::setDeclination(string decl) {
	nmeaINFO.declination = atof(decl.c_str());
}

void NmeaParser::setPDOP(string pdop) {
}

void NmeaParser::setVDOP(string vdop) {
}

#endif /* NmeaParser_CPP_ */
