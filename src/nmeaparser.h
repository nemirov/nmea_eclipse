// NMEAParser.h: interface for the NMEAParser class.
//
//////////////////////////////////////////////////////////////////////

#ifndef NMEAPARSER_H
#define NMEAPARSER_H

//#include "GPSInfo.h"

#include <string>
#include <vector>

using namespace std;

class NmeaParser {
public:
	NmeaParser();
	virtual ~NmeaParser();
	void Parse(string *nmeaString);
	//GPSInfo & GetActualGPSInfo();

	struct _nmeaINFO {

		/**
		 * Summary GPS information from all parsed packets,
		 * used also for generating NMEA stream
		 * @GGA2info,  XXX...2info
		 */
		int smask; /**< Mask specifying types of packages from which data have been obtained */

		int nav_system; /**< Navigation system from header NMEA message GP, GL, GN */

		int utc_year; /**< Years since 1900 */
		int utc_mon; /**< Months since January - [0,11] */
		int utc_day; /**< Day of the month - [1,31] */
		int utc_hour; /**< Hours since midnight - [0,23] */
		int utc_min; /**< Minutes after the hour - [0,59] */
		int utc_sec; /**< Seconds after the minute - [0,59] */
		int utc_hsec; /**< Hundredth part of second - [0,99] */

		int fix; /**< GPS quality indicator (0 = Invalid; 1 = GPS fix (SPS) ; 2 = DGPS fix, 3 = PPS fix,
					4 = Real Time Kinematic, 5 = Float RTK, 6 = estimated (dead reckoning), 7 = Manual input mode
					8 = Simulation mode) */


		int lat_deg; /**< Latitude - [degree] */
		int lat_min; /**< Latitude - [min] */
		int lat_sec; /**< Latitude - [sec] */
		string lat_hemisphere; /**Latitude hemisphere N or S*/

		int lon_deg; /**< Longitude - [degree] */
		int lon_min; /**< Longitude - [min] */
		int lon_sec; /**< Longitude - [sec] */
		string lon_hemisphere; /**Longitude hemisphere E or W*/

		int inview; /**< Total number of satellites in view */

		double HDOP; /**< Horizontal Dilution Of Precision */

		double elv; /**< Antenna altitude above/below mean sea level (geoid) in meters */
		string elv_meter; /**meters above mean sea level*/

		double height_of_geode;/**Height of geoid above WGS84 ellipsoid*/
		string geode_meter;/**meters Height*/


		double speed; /**< Speed over the ground in kilometers/hour */
		double direction; /**< Cource in degrees True */
		double declination; /**< Magnetic variation degrees (Easterly var. subtracts from true course) */



		double PDOP; /**< Position Dilution Of Precision */
		double VDOP; /**< Vertical Dilution Of Precision */

		/**
		 * Information about all satellites in view
		 * @see nmeaGPGSV
		 */
		int inuse; /**< Number of satellites in use (not those in view) */


		int id; /**< Satellite PRN number */
		int in_use; /**< Used in position fix */
		int elv_sat; /**< Elevation in degrees, 90 maximum */
		int azimuth; /**< Azimuth, degrees from true north, 000 to 359 */
		int sig_sat; /**< Signal, 00-99 dB */

	} nmeaINFO;


private:

	void ParseRecursive(const char ch);

	void GGA2Info();
	void ProcessGPGSA(const char *buf, const unsigned int bufSize);
	void ProcessGPGSV(const char *buf, const unsigned int bufSize);
	void ProcessGPRMB(const char *buf, const unsigned int bufSize);
	void RMC2Info();
	void ProcessGPZDA(const char *buf, const unsigned int bufSize);

	//bool m_logging;
	//GPSInfo m_GPSInfo;

	vector<string> *nmeaArray;

	int checkCrc(string *message);
	void setNavSystem(string system);
	void setDate(string date);
	void setTime(string time);
	void setLatitude(string latitude);
	void setLatHemisphere(string hem);
	void setLongitude(string longitude);
	void setLonHemisphere(string hem);
	void setFix(string fix);
	void setInview(string inview);
	void setHDOP(string hdop);
	void setElv(string elv);
	void setElvMeter(string meter);
	void setHeightOfGeode(string height);
	void setHeightMeter(string meter);
	void setSpeed(string speed);
	void setDirection(string direction);
	void setDeclination(string decl);
	void setPDOP(string pdop);
	void setVDOP(string vdop);
	//!!!!!!!!!!!!!!!!!!!!!!!!!!
	//create array satellite
	//!!!!!!!!!!!!!!!!!!!!!!!!!!
	void setInuse(string inuse);

};

#endif // NMEAPARSER_H
