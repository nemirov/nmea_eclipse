
/*
 * common.c
 *
 *  Created on: Jan 28, 2015
 *      Author: nemirov
 */
#include <iostream>
#include <string>
#include <string.h>
#include <cstdlib>
#include <vector>

using namespace std;

/******************************************************************************
 *                                                                            *
 * Function: gps_ltrim                                                        *
 *                                                                            *
 * Purpose: Strip characters from the beginning of a string                   *
 *                                                                            *
 * Parameters: str - string for processing                                    *
 *             charlist - null terminated list of characters                  *
 *                                                                            *
 * Return value:                                                              *
 *                                                                            *                                        *
 *                                                                            *
 ******************************************************************************/
void gps_ltrim(string * str, string *charlist) {
    string::const_iterator it = str->begin();
    while (it != str->end() && (string::npos != charlist->find_first_of(*it))) {
        str->erase(0, 1);
    }
}

/******************************************************************************
 *                                                                            *
 * Function: gps_rtrim                                                        *
 *                                                                            *
 * Purpose: Strip characters from the end of a string                         *
 *                                                                            *
 * Parameters: str - string for processing                                    *
 *             charlist - null terminated list of characters                  *
 *                                                                            *
 * Return value:                                  							  *
 *                                                                            *
 *                                                                            *
 ******************************************************************************/
void gps_rtrim(string * str, string *charlist) {
    string::const_reverse_iterator it = str->rbegin();
    while (it != str->rend() && (string::npos != charlist->find_first_of(*it))) {
        str->erase(str->length() - 1);
        it++;
    }
}

/******************************************************************************
 *                                                                            *
 * Function: gps_split_param                                                  *
 *                                                                            *
 * Purpose: Split string                                                      *
 *                                                                            *
 * Parameters: str - string for processing                                    *
 *             param - first value in string                                  *
 *             value - second value in string								  *
 *             separator - separator param and value						  *
 *                                                                            *
 * Return value: 0 - success                                                  *
 * 				-1 - bad string
 *                                                                            *
 *                                                                            *
 ******************************************************************************/

int gps_split_param(string str, vector<string> *value, string *sep) {

    int count = 0;
    string::size_type position = str.find_first_of(*sep);

    if (position != string::npos) {
        while (position != string::npos) {
            count++;
            value->push_back(str.substr(0, position));
            str.erase(0, position + 1);
            position = str.find_first_of(*sep);
        }

        value->push_back(str);
        return count + 1;
    } else {
        return -1;
    }
}

/******************************************************************************
 *                                                                            *
 * Function: gps_lrtrim                                                       *
 *                                                                            *
 * Purpose: Removes leading and trailing characters from the specified        *
 *          character string                                                  *
 *                                                                            *
 * Parameters: str      - [IN/OUT] string for processing                      *
 *             charlist - [IN] null terminated list of characters             *
 *                                                                            *
 ******************************************************************************/
void gps_lrtrim(string *str, string *charlist) {
    gps_rtrim(str, charlist);
    gps_ltrim(str, charlist);
}

/******************************************************************************
 *                                                                            *
 * Function: str2ulonglong                                                    *
 *                                                                            *
 * Purpose: convert string to 64bit unsigned long long                        *
 *                                                                            *
 * Parameters: str   - string to convert                                      *
 *             value - a pointer to converted value                           *
 *                                                                            *
 * Return value:  SUCCEED - the string is unsigned integer                    *
 *                FAIL - otherwise                                            *
 *                                                                            *
 *                                                                            *
 *                                                                            *
 * Comments: the function automatically processes suffixes K, M, G, T         *
 *                                                                            *
 ******************************************************************************/

int str2ulonglong(string *str, unsigned long long *value) {

    unsigned long long ull = strtoull(str->c_str(), NULL, 0);
    *value = ull;
    return 0;
}

/******************************************************************************
 *                                                                            *
 * Function: gps_trim_str_list                                                *
 *                                                                            *
 * Purpose: remove whitespace surrounding a string list item delimiters       *
 *                                                                            *
 * Parameters: list      - the list (a string containing items separated by   *
 *                         delimiter)                                         *
 *             delimiter - the list delimiter                                 *
 *                                                                            *
 *						                                                      *
 *                                                                            *
 ******************************************************************************/

void gps_trim_str_list(string *str, string *delimiter) {

    string tmp = "";
    string whitespace = " \t";
    string tmp_substr;

    gps_lrtrim(str, delimiter);

    tmp.swap(*str);

    string::size_type position = tmp.find(*delimiter);

    while (position != string::npos) {

        tmp_substr = tmp.substr(0, position);

        gps_ltrim(&tmp_substr, &whitespace);
        gps_rtrim(&tmp_substr, &whitespace);

        *str += tmp_substr + ",";
        tmp.erase(0, position + 1);
        position = tmp.find(*delimiter);
    }

    gps_lrtrim(&tmp, &whitespace);
    *str += tmp;

}


