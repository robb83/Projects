// gcc -Wall -O2 -o time time.c scanf_rfc3339.c
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "scanf_rfc3339.h"

int main(int argc, char** argv) {
	
	printf("%d\n", (int)simple_rfc3339("2014-07-02T10:18:00Z"));
	printf("%d\n", (int)simple_rfc3339("2014-07-02T10:18:00+00:00"));
	printf("%d\n", (int)simple_rfc3339("2014-07-02T11:18:00+01:00"));
	printf("%d\n", (int)simple_rfc3339("2014-07-02T12:18:00+02:00"));
	printf("%d\n", (int)simple_rfc3339("2014-01-01T00:00:00+01:00"));
	
	// TZ
	char* tz1 = getenv("TZ");
	if (tz1 == NULL) printf("Environment TZ: NULL\n");
	else printf("Environment TZ: %s\n", tz1);
	
	// gettimeofday
	struct timeval tv;
	struct timezone tz;
	if (gettimeofday(&tv, &tz)  == 0) {
		printf("gettimeofday:\t%lu %d %d\n", tv.tv_sec, tz.tz_minuteswest, tz.tz_dsttime);
	} else {
		printf("gettimeofday:\t-\n");
	}
	
	struct tm *a;
	
	// time
	time_t t1 = time(NULL);
	printf("time:\t\t%lu\t\t%s\n", t1, ctime(&t1));
	
	a = localtime(&t1);
	time_t t2 = mktime(a);
	printf("localtime:\t%lu\t%s\t%s\n", t2, ctime(&t2), asctime(a));	
	
	a = gmtime(&t1);
	time_t t3 = mktime(a);
	printf("gmtime:\t\t%lu\t%s\t%s\n", t3, ctime(&t3), asctime(a));	
	
	// custom struct tm
	struct tm timeinfo;
	
	timeinfo.tm_year = 114;
	timeinfo.tm_mon = 6;
	timeinfo.tm_mday = 1;
	
	timeinfo.tm_hour = 22;
	timeinfo.tm_min	 = 22;
	timeinfo.tm_sec = 0;
	
	timeinfo.tm_isdst = -1;
	timeinfo.tm_wday = 0;
	timeinfo.tm_yday = 0;
	
	timeinfo.tm_gmtoff = 0;
	timeinfo.tm_zone = NULL;
	
	int offset, sec;
	
	offset = (int)timeinfo.tm_gmtoff;
	sec = (int)mktime(&timeinfo);
	
	printf("mktime offset %d: \t%d\t%d\n", offset, sec, (int)timeinfo.tm_gmtoff);

	timeinfo.tm_gmtoff = 1 * 60 * 60;
	
	offset = (int)timeinfo.tm_gmtoff;
	sec = (int)mktime(&timeinfo);
	
	printf("mktime offset %d: \t%d\t%d\n", offset, sec, (int)timeinfo.tm_gmtoff);

	return 0;
}
