#include "scanf_rfc3339.h"
#include <stdio.h>

#define ISDIGIT(v) (v >= '0' && v <= '9')

time_t simple_rfc3339(char* string) {
	int dy, dm, dd;
	int th, tm, ts;
	int oh, om, osign;
	char current;
	
	if (!string)
		return (time_t)0;
	
	// date
	if (sscanf(string, "%04d-%02d-%02d", &dy, &dm, &dd) == 3) {
		string += 10;
		
		if (*string++ != 'T')
			return (time_t)0;
		
		// time
		if (sscanf(string, "%02d:%02d:%02d", &th, &tm, &ts) == 3) {
			string += 8;
			
			current = *string;
			
			// optional: second fraction
			if (current == '.') {
				++string;
				while(ISDIGIT(*string))
					++string;
					
				current = *string;
			}
			
			if (current == 'Z') {
				oh = om = 0;
				osign = 1;
			} else if (current == '-') {
				++string;
				if (sscanf(string, "%02d:%02d", &oh, &om) != 2)
					return (time_t)0;
				osign = -1;
			} else if (current == '+') {
				++string; 
				if (sscanf(string, "%02d:%02d", &oh, &om) != 2)
					return (time_t)0;
				osign = 1;
			} else {
				return (time_t)0;
			}
			
			struct tm timeinfo;
			timeinfo.tm_wday = timeinfo.tm_yday = 0;
			timeinfo.tm_zone = NULL;
			timeinfo.tm_isdst = -1;
			
			timeinfo.tm_year = dy - 1900;
			timeinfo.tm_mon = dm - 1;
			timeinfo.tm_mday = dd;
			
			timeinfo.tm_hour = th;
			timeinfo.tm_min = tm;
			timeinfo.tm_sec = ts;
			
			// convert to utc
			return timegm(&timeinfo) - (((oh * 60 * 60) + (om * 60)) * osign);
		}
	}
	
	return (time_t)0;
}
