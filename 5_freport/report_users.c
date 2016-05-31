#include "freport.h"

static const int UID_ROOT = 0;
static const int RANGE_USERS[2] = {1000, 65533};  
// below 1000: system accouts who never login; 
// 65534(max): nobody

int report_users( int days )
{
	char cmdstring[200];
	char *reportfile = "lastlog.report";

//lastlog -b days -u 1000-65533   > lastlog.report
//lastlog -b days -u 0 >> lastlog.report
	sprintf( cmdstring, "lastlog -b %d -u %d", days, UID_ROOT );
	if ( system(cmdstring) < 0 )
	{
		fprintf(stderr, "error executing lastlog. exit...\n");
		return -1;
	}

	sprintf( cmdstring, "lastlog -b %d -u %d-%d > %s", days, RANGE_USERS[0], RANGE_USERS[1], reportfile );
	if ( system(cmdstring) < 0 )
	{
		fprintf(stderr, "error executing lastlog. exit...\n");
		return -2;
	}

	sprintf( cmdstring, "grep -v \"Port\" %s", reportfile );
	if ( system(cmdstring) < 0 )
	{
		fprintf(stderr, "error executing lastlog. exit...\n");
		return -3;
	}

	unlink(reportfile);
	return 0;
}
