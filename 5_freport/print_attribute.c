#include "freport.h"

static const int NO_ACC_SECS = 30 * 24 * 60 * 60;  // no access in the last 30 days
static const int BYTES_BEYOND = 1000000;   //1M
static const int ACC_BETW[2] = { 7 * 24*60*60, 14 * 24*60*60 };   // files accessed during the week before last

int A_pr_attb( char *fullname, struct stat *buf, struct timespec *now, int opt ) 
{
	uid_t uid = geteuid();
	char *cmdstring;
	char filetmp[100];
	int status;
	struct stat buf_report;

	long long int dates_back_acc;
	dates_back_acc = now->tv_sec - buf->st_atim.tv_sec;

	printf("%s :", fullname);

	if( dates_back_acc > NO_ACC_SECS )
		printf(" long time no access,");
	else
		printf("%21s",",");

	if( buf->st_size > BYTES_BEYOND  )
		printf(" huge file,");
	else
		printf("%11s",",");

	if( ( dates_back_acc > ACC_BETW[0] ) && ( dates_back_acc < ACC_BETW[1]  )  )
		printf(" accessed during specific period,");
	else
		printf("%33s",",");

// being read by other users ?
// lsof -u ^1000 filename > report_tmp
	cmdstring = malloc( strlen(fullname) + 200 );
	sprintf( cmdstring, "lsof -e /run/user/1000/gvfs -u ^%d -f -- ", uid );
	strcat( cmdstring, fullname ); 
	strcat( cmdstring, " > " );
	snprintf( filetmp, 99, "report_tmp_%ld_%d", now->tv_sec, getpid() );
	strcat( cmdstring, filetmp);

	if( ( status = system(cmdstring) ) < 0 )
		printf("system ERROR getting whether being read by other users...");
	else
	{
		if( stat( filetmp, &buf_report) == -1 )
			printf("stat ERROR getting size of result of lsof...");
		else if ( buf_report.st_size > 0 )
			printf("being read by other users!!!");
	}
	unlink( filetmp );
	free(cmdstring);

	printf("\n");

	return 0;
}


//if size beyond "opt" bytes, printf filename
int B_s_pr_huge( char *fullname, struct stat *buf, struct timespec *now, int opt )
{
	if( buf->st_size > opt )  
		printf("%s\n", fullname);
	return 0;
}


// if accessed within "opt" days, printf filename
int B_a_pr_acced( char *fullname, struct stat *buf, struct timespec *now, int opt ) 
{
	if( ( now->tv_sec - buf->st_atim.tv_sec ) < opt * 24 * 60 * 60 )
		printf("%s\n", fullname );
	return 0;
}
