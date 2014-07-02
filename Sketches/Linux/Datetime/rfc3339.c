#include "rfc3339.h"
#include <stdio.h>

#define ISDIGIT(v) (v >= '0' && v <= '9')

static char* __rfc3339_datetime(struct tm* timeinfo, char* string);
static char* __rfc3339_time(struct tm* timeinfo, char* string);
static char* __rfc3339_offset(int *offset, char* string);

time_t rfc3339(char* string) {
	struct tm timeinfo;
	int offset;
	
	if (!string) return (time_t)0;
	
	string = __rfc3339_datetime(&timeinfo, string);
	
	if (!string || (*string++ != 'T')) return (time_t)0;
	
	string = __rfc3339_time(&timeinfo, string);
	
	if (!string) return (time_t)0;
	
	string = __rfc3339_offset(&offset, string);
	
	if (!string) return (time_t)0;
	
	timeinfo.tm_wday = timeinfo.tm_yday = 0;
	timeinfo.tm_zone = NULL;
	timeinfo.tm_isdst = -1;
	timeinfo.tm_gmtoff = 0;
	
	return timegm(&timeinfo) - offset;
}

static char* __rfc3339_datetime(struct tm* timeinfo, char* string) {

	char current;
	int temp, i;
	
	// year
	temp = 0;
	for ( i = 0; i < 4; ++i) {
		current = *string++;
		if (ISDIGIT(current))
			temp = temp * 10 + (current - '0');
		else 
			return NULL;
	}
	timeinfo->tm_year = temp - 1900; // year - 1900
	
	// -
	if ((current = *string++) != '-') 
		return NULL;
	
	// month
	temp = 0;
	for ( i = 0; i < 2; ++i) {
		current = *string++;
		if (ISDIGIT(current))
			temp = temp * 10 + (current - '0');
		else 
			return NULL;
	}
	timeinfo->tm_mon = temp - 1; // 00-11

	// -
	if ((current = *string++) != '-') 
		return NULL;
	
	// date
	temp = 0;
	for ( i = 0; i < 2; ++i) {
		current = *string++;
		if (ISDIGIT(current))
			temp = temp * 10 + (current - '0');
		else 
			return NULL;
	}
	timeinfo->tm_mday = temp; // 01-31
	
	return string;
}

static char* __rfc3339_time(struct tm* timeinfo, char* string) {

	char current;
	int temp, i;
	
	// hour
	temp = 0;
	for ( i = 0; i < 2; ++i) {
		current = *string++;
		if (ISDIGIT(current))
			temp = temp * 10 + (current - '0');
		else 
			return NULL;
	}
	timeinfo->tm_hour = temp; // 00-59
	
	// :
	if ((current = *string++) != ':') 
		return NULL;
	
	// minute
	temp = 0;
	for ( i = 0; i < 2; ++i) {
		current = *string++;
		if (ISDIGIT(current))
			temp = temp * 10 + (current - '0');
		else 
			return NULL;
	}
	timeinfo->tm_min = temp; // 00-59

	// :
	if ((current = *string++) != ':') 
		return NULL;
	
	// second
	temp = 0;
	for ( i = 0; i < 2; ++i) {
		current = *string++;
		if (ISDIGIT(current))
			temp = temp * 10 + (current - '0');
		else 
			return NULL;
	}
	timeinfo->tm_sec = temp; // 00-60
	
	// skip second fraction
	current = *string;
	if (current == '.') {
		current = *(++string);
		
		while(ISDIGIT(current)) {
			current = *(++string);
		}
	}
	
	return string;
}

static char* __rfc3339_offset(int* offset, char* string) {

	char current;
	int offset_sign, temp, i;
	
	current = *string;
	
	if (current == 'Z') {

		*offset = 0;
		return ++string;
	} else if (current == '+') {
		
		offset_sign = 1;
	} else if (current == '-') {
		
		offset_sign = -1;
	} else {
		
		return NULL;
	}
	
	++string;

	// hour
	temp = 0;
	for ( i = 0; i < 2; ++i) {
		current = *string++;
		if (ISDIGIT(current))
			temp = temp * 10 + (current - '0');
		else 
			return NULL;
	}
	*offset = temp * 60 * 60; // seconds
	
	// :
	if ((current = *string++) != ':') 
		return NULL;
	
	// minute
	temp = 0;
	for ( i = 0; i < 2; ++i) {
		current = *string++;
		if (ISDIGIT(current))
			temp = temp * 10 + (current - '0');
		else 
			return NULL;
	}
	*offset += temp * 60; // seconds
	*offset *= offset_sign;

	return string;
}
