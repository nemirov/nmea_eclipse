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
#include <algorithm>
#include <math.h>
#include <typeinfo>

using namespace std;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

NmeaParser::NmeaParser() {
	nmeaArray = new vector<string>;
	//default system is GPS
	nmeaINFO = &nmeaINFOAll[NMEA_NAV_SYSTEM_GPS];
}

NmeaParser::~NmeaParser() {

}

void NmeaParser::Parse(string *nmeaString) {

	string gps_cfg_rtrim_chars = "$ \r\t\n";

	nmeaArray->clear();

	cout << *nmeaString << endl;

	gps_lrtrim(nmeaString, &gps_cfg_rtrim_chars);

	if (checkCrc(nmeaString) != -1) {

		if (nmeaArray->at(0).size() != 5
				|| setNavSystem(nmeaArray->at(0)) == -1) {
			return;
		}

		//Sentence Codes
		string sentence_codes = nmeaArray->at(0).substr(2, 3);

		if (sentence_codes == string("GGA") && nmeaArray->size() == 15) {

			GGA2Info();

		} else if (sentence_codes == string("RMC") && nmeaArray->size() == 13) {

			RMC2Info();

		} else if (sentence_codes == string("ZDA")) {

			ZDA2Info();

		} else if (sentence_codes == string("GSV")) {

			GSV2Info();

		} else if (sentence_codes == string("GLL")) {

			GLL2Info();

		}
		/*
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
 Format

 eg2. $GPGGA,hhmmss.ss,ddmm.mmm,a,dddmm.mmm,b,q,xx,p.p,a.b,M,c.d,M,x.x,nnnn

 hhmmss.ss = UTC of position
 ddmm.mmm = latitude of position
 a = N or S, latitutde hemisphere
 dddmm.mmm = longitude of position
 b = E or W, longitude hemisphere
 q = GPS Quality indicator (0=No fix, 1=Non-differential GPS fix, 2=Differential GPS fix, 6=Estimated fix)
 xx = number of satellites in use
 p.p = horizontal dilution of precision
 a.b = Antenna altitude above mean-sea-level
 M = units of antenna altitude, meters
 c.d = Geoidal height
 M = units of geoidal height, meters
 x.x = Age of Differential GPS data (seconds since last valid RTCM transmission)
 nnnn = Differential reference station ID, 0000 to 1023

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

	//Age of Differential GPS data (seconds since last valid RTCM transmission)*/
	setAgeGps(nmeaArray->at(13));

	//Differential reference station ID, 0000 to 1023*/
	setRefStation(nmeaArray->at(14));

}

/*
 Format

 for NMEA 0183 version 3.00 active the Mode indicator field is added
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

		//E or W of magnetic variation
		setMagneticVariation(nmeaArray->at(11));

		//if NMEA 0183 version >= 3.00
		if (nmeaArray->size() == 13) {
			//Mode indicator, (A=Autonomous, D=Differential, E=Estimated, N=Data not valid)
			setFixRMC(nmeaArray->at(12));
		};

	} else {
		return;
	}
}

/*
 Format
 ZDA - Data and Time

 $GPZDA,hhmmss.ss,dd,mm,yyyy,xx,yy*CC
 $GPZDA,201530.00,04,07,2002,00,00*60

 where:
 hhmmss    HrMinSec(UTC)
 dd,mm,yyy Day,Month,Year
 xx        local zone hours -13..13
 yy        local zone minutes 0..59
 *CC       checksum
 */

void NmeaParser::ZDA2Info() {
	// Time
	setTime(nmeaArray->at(1));

	//Day
	setDay(nmeaArray->at(2));

	//Month
	setMonth(nmeaArray->at(3));

	//Year
	setYear(nmeaArray->at(4));

	//Local hours
	setLocalHours(nmeaArray->at(5));

	//Local min
	setLocalMin(nmeaArray->at(6));

	cout << "ZDA: " << "time: " << nmeaINFO->utc_hour << ":"
			<< nmeaINFO->utc_min << ":" << nmeaINFO->utc_sec << endl;
	cout << "ZDA: " << "date: " << nmeaINFO->utc_day << ":" << nmeaINFO->utc_mon
			<< ":" << nmeaINFO->utc_year << endl;
	cout << "ZDA: " << "local: " << nmeaINFO->local_zone_hours << ":"
			<< nmeaINFO->local_zone_min << endl;

}

/*Format
 GLL - Geographic Position - Latitude/Longitude
 This is one of the sentences commonly emitted by GPS units.

 1       2 3        4 5         6 7   8
 |       | |        | |         | |   |
 $--GLL,llll.ll,a,yyyyy.yy,a,hhmmss.ss,a,m,*hh<CR><LF>
 Field Number:

 Latitude

 N or S (North or South)

 Longitude

 E or W (East or West)

 Universal Time Coordinated (UTC)

 Status A - Data Valid, V - Data Invalid

 FAA mode indicator (NMEA 2.3 and later)

 Checksum
 */
void NmeaParser::GLL2Info() {

	if (nmeaArray->at(6) == string("A")) {

		//Latitude
		setLatitude(nmeaArray->at(1));

		//Latitude hemisphere
		setLatHemisphere(nmeaArray->at(2));

		//Longitude
		setLongitude(nmeaArray->at(3));

		//Longitude hemisphere
		setLonHemisphere(nmeaArray->at(4));

		//Time
		setTime(nmeaArray->at(5));

		cout << "GLL: " << "time: " << nmeaINFO->utc_hour << ":"
				<< nmeaINFO->utc_min << ":" << nmeaINFO->utc_sec << endl;
		cout << "GLL: " << "longitude:  " << nmeaINFO->lon_deg << ":"
				<< nmeaINFO->lon_min << ":" << nmeaINFO->lon_sec
				<< nmeaINFO->lon_hemisphere << endl;
		cout << "ZDA: " << "latitude: " << nmeaINFO->lat_deg << ":"
				<< nmeaINFO->lat_min << ":" << nmeaINFO->lat_sec
				<< nmeaINFO->lat_hemisphere << endl;

	}
}

/*
 Format

 GPS DOP and active satellites

 eg1. $GPGSA,A,3,,,,,,16,18,,22,24,,,3.6,2.1,2.2*3C
 eg2. $GPGSA,A,3,19,28,14,18,27,22,31,39,,,,,1.7,1.0,1.3*34


 1    = Mode:
 M=Manual, forced to operate in 2D or 3D
 A=Automatic, 3D/2D
 2    = Mode:
 1=Fix not available
 2=2D
 3=3D
 3-14 = PRN's of Satellite Vechicles (SV's) used in position fix (null for unused fields)
 15   = Position Dilution of Precision (PDOP)
 16   = Horizontal Dilution of Precision (HDOP)
 17   = Vertical Dilution of Precision (VDOP)
 */

void GSA2Info() {

}
/*
 Format
 GPS Satellites in view

 eg. $GPGSV,3,1,11,03,03,111,00,04,15,270,00,06,01,010,00,13,06,292,00*74
 $GPGSV,3,2,11,14,25,170,00,16,57,208,39,18,67,296,40,19,40,246,00*74
 $GPGSV,3,3,11,22,42,067,42,24,14,311,43,27,05,244,00,,,,*4D


 $GPGSV,1,1,13,02,02,213,,03,-3,000,,11,00,121,,14,13,172,05*62


 1    = Total number of messages of this type in this cycle
 2    = Message number
 3    = Total number of SVs in view
 4    = SV PRN number
 5    = Elevation in degrees, 90 maximum
 6    = Azimuth, degrees from true north, 000 to 359
 7    = SNR, 00-99 dB (null when not tracking)
 8-11 = Information about second SV, same as field 4-7
 12-15= Information about third SV, same as field 4-7
 16-19= Information about fourth SV, same as field 4-7

 */
void NmeaParser::GSV2Info() {

}

void NmeaParser::ProcessGPGSA(const char *buf, const unsigned int bufSize) {

}

void NmeaParser::ProcessGPRMB(const char *buf, const unsigned int bufSize) {

}

/*
 * Functions settings
 */
string itoa_custom(int value, int base) {

	string buf;

	// check that the base if valid
	if (base < 2 || base > 16)
		return buf;

	enum {
		kMaxDigits = 35
	};
	buf.reserve(kMaxDigits); // Pre-allocate enough space.

	int quotient = value;

	// Translating number to string with base:
	do {
		buf += "0123456789abcdef"[abs(quotient % base)];
		quotient /= base;
	} while (quotient);

	// Append the negative sign
	if (value < 0)
		buf += '-';

	std::reverse(buf.begin(), buf.end());
	return buf;
}

int NmeaParser::checkCrc(string *message) {
	string separator_value = ",";

	unsigned int i = 0;
	int calc_crc = 0;

	while (message->at(i) != '*') {
		if (i == message->size()) {
			return -1;
		};
		calc_crc ^= message->at(i);
		i++;
	}

	string crc = message->substr(i + 1, i + 2);
	transform(crc.begin(), crc.end(), crc.begin(), ::tolower);

	if (crc != itoa_custom(calc_crc, 16)) {
		return -1;
	}

	*message = message->substr(0, i);
	if (gps_split_param(*message, nmeaArray, &separator_value) == -1) {
		return -1;
	}

	return 0;

}
int NmeaParser::setNavSystem(string system) {

	string nav_system = system.substr(0, 2);

	if (nav_system == string("GP")) {
		nmeaINFO = &nmeaINFOAll[NMEA_NAV_SYSTEM_GPS];
		nmeaINFO->nav_system = NMEA_NAV_SYSTEM_GPS;
	} else if (nav_system == string("GL")) {
		nmeaINFO = &nmeaINFOAll[NMEA_NAV_SYSTEM_GLONASS];
		nmeaINFO->nav_system = NMEA_NAV_SYSTEM_GLONASS;
	} else if (nav_system == string("GN")) {
		nmeaINFO = &nmeaINFOAll[NMEA_NAV_SYSTEM_GPS_GLONASS];
		nmeaINFO->nav_system = NMEA_NAV_SYSTEM_GPS_GLONASS;
	} else {
		return -1;
	}
	return 0;
}

void NmeaParser::setDate(string date) {
	if (date.size() > 6) {
		nmeaINFO->utc_day = atoi(date.substr(0, 2).c_str());
		nmeaINFO->utc_mon = atoi(date.substr(2, 2).c_str());
		nmeaINFO->utc_year = atoi(date.substr(4, 2).c_str());
	}
}

void NmeaParser::setDay(string day) {
	nmeaINFO->utc_day = atoi(day.c_str());
}

void NmeaParser::setMonth(string month) {
	nmeaINFO->utc_mon = atoi(month.c_str());
}

void NmeaParser::setYear(string year) {
	nmeaINFO->utc_year = atoi(year.c_str());
}

void NmeaParser::setTime(string time) {

	if (time.size() > 6) {
		nmeaINFO->utc_hour = atoi(time.substr(0, 2).c_str());
		nmeaINFO->utc_min = atoi(time.substr(2, 2).c_str());
		nmeaINFO->utc_sec = atoi(time.substr(4, 2).c_str());
	}
}

void NmeaParser::setLocalHours(string hours) {
	nmeaINFO->local_zone_hours = atoi(hours.c_str());
}

void NmeaParser::setLocalMin(string min) {
	nmeaINFO->local_zone_min = atoi(min.c_str());
}

void NmeaParser::setLatitude(string latitude) {
	string separator = ".";
	vector<string> tmp_array;
	if (gps_split_param(latitude, &tmp_array, &separator) != -1) {
		nmeaINFO->lat_deg = atoi(tmp_array.at(0).substr(0, 2).c_str());
		nmeaINFO->lat_min = atoi(tmp_array.at(0).substr(2, 2).c_str());
		nmeaINFO->lat_sec = floor(
				atof((string("0.") + tmp_array.at(1)).c_str()) * 60 + 0.5);

	};
}

void NmeaParser::setLatHemisphere(string hem) {
	nmeaINFO->lat_hemisphere = hem;
}

void NmeaParser::setLongitude(string longitude) {
	string separator = ".";
	vector<string> tmp_array;
	if (gps_split_param(longitude, &tmp_array, &separator) != -1) {
		nmeaINFO->lon_deg = atoi(tmp_array.at(0).substr(0, 3).c_str());
		nmeaINFO->lon_min = atoi(tmp_array.at(0).substr(3, 2).c_str());
		nmeaINFO->lon_sec = floor(
				atof((string("0.") + tmp_array.at(1)).c_str()) * 60 + 0.5);

	};
}

void NmeaParser::setLonHemisphere(string hem) {
	nmeaINFO->lon_hemisphere = hem;
}

void NmeaParser::setFix(string fix) {
	nmeaINFO->fix = atoi(fix.c_str());
}

void NmeaParser::setFixRMC(string mode) {
	if (mode == string("A")) {
		nmeaINFO->fix = 1; //GPS fix (SPS)
	} else if (mode == string("D")) {
		nmeaINFO->fix = 2; //DGPS fix
	} else if (mode == string("E")) {
		nmeaINFO->fix = 6; //estimated (dead reckoning)
	} else {
		nmeaINFO->fix = 0; //Invalid
	};
}
void NmeaParser::setInview(string inview) {
	nmeaINFO->inview = atoi(inview.c_str());
}

void NmeaParser::setHDOP(string hdop) {
	nmeaINFO->HDOP = atof(hdop.c_str());
}

void NmeaParser::setElv(string elv) {
	nmeaINFO->elv = atof(elv.c_str());
}

void NmeaParser::setElvMeter(string meter) {
	nmeaINFO->elv_meter = meter;
}

void NmeaParser::setHeightOfGeode(string height) {
	nmeaINFO->height_of_geode = atof(height.c_str());
}

void NmeaParser::setHeightMeter(string meter) {
	nmeaINFO->geode_meter = meter;
}

void NmeaParser::setAgeGps(string age) {
	if (age.size() > 0) {
		nmeaINFO->age_dgps = atoi(age.c_str());
	}
	nmeaINFO->age_dgps = 0;
}

void NmeaParser::setRefStation(string station) {
	if (station.size() > 0) {
		nmeaINFO->age_dgps = atoi(station.c_str());
	}
	nmeaINFO->age_dgps = 0;
}
void NmeaParser::setSpeed(string speed) {
	nmeaINFO->speed = atof(speed.c_str()) * 1.852;
}

void NmeaParser::setDirection(string direction) {
	nmeaINFO->direction = atof(direction.c_str());
}

void NmeaParser::setDeclination(string decl) {
	nmeaINFO->declination = atof(decl.c_str());
}

void NmeaParser::setMagneticVariation(string magnetic_var) {
	nmeaINFO->magnetic_variation = magnetic_var;
}

void NmeaParser::setPDOP(string pdop) {
}

void NmeaParser::setVDOP(string vdop) {
}

#endif /* NmeaParser_CPP_ */
