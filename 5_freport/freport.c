#include "freport.h"

int main( int argc, char *argv[] )
{

	const struct option long_options[] = {
		{ "accessed", 1, NULL, 'a' },
		{ "size-beyond", 1, NULL, 's' },
		{ "users-nologin", 1, NULL, 'u' },
		{ NULL, 0, NULL, 0 } 
	};
	const char *short_options = "a:s:u:";
	int flag_options = 0;
	int flag_users = 0;
	int c;
	int opt = 0;
	struct timespec now;
	int (*myfun)( char *, struct stat *,  struct timespec *, int );
	int i;
	struct mynode *pnode, *cnode;

	if( argc < 2 )
	{
		printf("illegal input! example of legal arguments:\n");
		printf("./freport PATHNAME     //ls all files in PATHNAME and whether has specific features\n");
		printf("./freport --accessed 3 PATHNAME  //ls files in PATHNAME that have been accessed with 3 days\n");
		printf("./freport --size-beyond 1000000 PATHNAME  //ls files in PATHNAME whose size is bigger than 1MB\n");
		printf("./freport --users-nologin 3 //list users who has never logged in in the last 3 days\n");
		printf("exit.... please try again....\n\n");
		exit(-1);
	}

	while ( ( c = getopt_long(argc, argv, short_options, long_options, NULL ) ) != -1 )
	{
		flag_options = 1;
		switch(c) 
		{
			case 'a':
				opt = atoi(optarg);
				printf("==== report files have been accessed within %d days ====\n", opt);
				myfun = B_a_pr_acced;
				break;
			case 's':
				opt = atoi(optarg);
				printf("==== report files with size beyond %d bytes ====\n", opt);
				myfun = B_s_pr_huge;
				break;
			case 'u':
				opt = atoi(optarg);
				printf("=== report users who has never logged in in the last %d days ===\n", opt);
				flag_users = 1;
				break;
			default:
				printf("invalid options...\n");
		}
	}
	
	if ( ! flag_options )
	{
		printf("no options, executing part A--print all files.   report:\n");
		printf("\"long time no access\" if not accessed in the last 30 days.\n");
		printf("\"huge file\" if size beyond 1M bytes.\n");
		printf("\"accessed during specific period\" if accessed during the week before last.\n");
		printf("\"being read\" if is being read by other users.\n\n");
		myfun = A_pr_attb;
	}

	if( flag_users )
	{
		if( (i = report_users(opt)) != 0 )
			fprintf( stderr, "error report_users: error %d\n", i );
		exit(0);
	}

	printf("================= STARTPOINT =%s\n====================\n\n", argv[optind] );

	if( clock_gettime(CLOCK_REALTIME, &now) == -1 )
	{
		printf("cannot get current time, exit...\n");
		exit(-2);
	}

	for (i=0; i<100; i++)
		hashlist[i] = NULL;

	traverse( argv[optind], myfun, &now, opt);	

	// free hashlist
	for(i=0; i<100; i++)
		if ( hashlist[i] != NULL )
		{
			pnode = hashlist[i];
			while( pnode->next != NULL)
			{
				cnode = pnode->next;
				free(pnode);
				pnode = cnode;
			}
			free(pnode);
		}

	exit(0);
}
