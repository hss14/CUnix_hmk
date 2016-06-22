#include "../chatroom.h"

int pr_time(char *buf, size_t maxsize)
{
	time_t calptr;
	struct tm *tmp;

	if( time(&calptr) == -1 )
		return -1;
	if( ( tmp = localtime(&calptr) ) == NULL )
		return -1;
	if( strftime( buf, maxsize, "%F %A %X", tmp ) == 0 )
		return -1;
	return 0;
}
