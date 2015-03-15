/*
 * common.h
 *
 *  Created on: Jan 28, 2015
 *      Author: nemirov
 */

#ifndef COMMON_H_
#define COMMON_H_
#include <string>
#include <vector>

#define FALSE 0
#define TRUE 1

#define	TYPE_INT				0
#define	TYPE_STRING				1
#define	TYPE_MULTISTRING		2
#define	TYPE_UINT64				3
#define	TYPE_STRING_LIST		4
#define TYPE_IP					5


#define NMEA_SIG_BAD        (0)
#define NMEA_SIG_LOW        (1)
#define NMEA_SIG_MID        (2)
#define NMEA_SIG_HIGH       (3)

#define NMEA_NAV_SYSTEM_GPS				(0)
#define NMEA_NAV_SYSTEM_GLONASS			(1)
#define NMEA_NAV_SYSTEM_GPS_GLONASS		(2)
#define NMEA_NAV_SYSTEM_UNKNOWN			(3)

#define NMEA_FIX_BAD        (1)
#define NMEA_FIX_2D         (2)
#define NMEA_FIX_3D         (3)

#define NMEA_MAXSAT         (12)
#define NMEA_SATINPACK      (4)
#define NMEA_NSATPACKS      (NMEA_MAXSAT / NMEA_SATINPACK)

#define NMEA_DEF_LAT        (5001.2621)
#define NMEA_DEF_LON        (3613.0595)


using namespace std;

void				gps_rtrim(string *str, string *charlist);
void				gps_ltrim(string *str, string *charlist);
void				gps_lrtrim(string *str, string *charlist);
int 				gps_split_param(string str, vector<string> *value, string *sep);
int				 	str2ulonglong(string *str, unsigned long long *value);
void				gps_trim_str_list(string *list, string *delimiter);
double				str2double(const char *str);


#endif /* COMMON_H_ */
