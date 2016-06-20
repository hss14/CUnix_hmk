#include "csv8.h"

int csv2bin( const char *filecsv, const char *filebin )
{
	FILE *fpread, *fpwrite, *fpbad, *fplong;
	char *badrecords, *longrecords;
	char line[MaxLine];
	struct StuRecord stu;
	uint8_t stuname[MaxName];
	int flag = 0;
	int i;
	int par[NCHILD][2], chd[NCHILD][2];
	pid_t pid;
	int ichild;
	uint32_t cnt_record=0, line_csv=1;
	uint32_t cnt_succeed = 0;

#ifdef HSSDEBUG
	int childcnt=0;
	setbuf(stdout,NULL);
#endif

	badrecords = malloc( strlen(filecsv)+5 );
	strcpy(badrecords, filecsv);
	strcat(badrecords, ".err");

	longrecords = malloc( strlen(filecsv)+6 );
	strcpy(longrecords, filecsv);
	strcat(longrecords, ".long");

	unlink(badrecords);
	if( ( fpbad = fopen(badrecords, "a") ) == NULL )
	{
		fprintf(stdout, "csv2bin: error fopen create %s, exit because of error at the very beginning\n", badrecords);
		return -1;
	}
	unlink(longrecords);
	if( ( fplong = fopen(longrecords, "a") ) == NULL )
	{
		fprintf(fpbad, "csv2bin: error fopen create %s, exit because of error at the very beginning\n", longrecords);
		return -2;
	}
	if( (fpread = fopen(filecsv, "r")) == NULL )
	{
		fprintf(fpbad, "csv2bin: error fopen %s\n", filecsv );
		return -3;
	}
	unlink(filebin);
	if( (fpwrite = fopen(filebin, "wb")) == NULL )
	{
		fprintf(fpbad, "csv2bin: error fopen %s\n", filebin );
		return -4;
	}

	for( i=0; i<NCHILD; i++)
	{
		pipe( par[i] );
		pipe( chd[i] );
		if( (pid = fork()) == -1 )
		{
			fprintf(fpbad, "csv2bin: error fork while i = %d", i);
			return -5;
		}
		else if( pid > 0 )  // parent
		{
			close( par[i][0] );
			close( chd[i][1] );
		}
		else  //child
		{
			dup2( par[i][0], STDIN_FILENO);
			dup2( chd[i][1], STDOUT_FILENO);
			close( par[i][1] ); close( par[i][0] );
			close( chd[i][1] ); close( chd[i][0] );
			break;
		}
	}

	if( pid > 0 )   ////////////////// parent: get a line and poll to child 1 ~ 5 ////////////////////////// 
	{
		// discard the first line
		if( fgets(line, MaxLine, fpread) == NULL )
		{
			fprintf(fpbad, "csv2bin: error fgets at the first line\n");
			return -6;
		}
		while( ( strlen(line)==MaxLine-1) && ( line[MaxLine-2]!='\n' ) )   
		{         // if real length of line > MaxLine: dicard this line; always check and discard each time fgets
			flag = 1;
			fputs( line, fplong);
			if( fgets(line, MaxLine, fpread) == NULL )
			{
				fprintf(fpbad, "csv2bin: error fgets at the first line\n");
				return -7;
			}
		}
		if( flag)
		{
			fputs( line, fplong);
			fprintf(fplong, "TOO LONG the first line!\n\n");
		}
	
		if( fwrite( &cnt_succeed, sizeof(uint32_t), 1, fpwrite)!=1 )
		{
			fprintf(fpbad, "csv2bin: error fwrite cnt_succeed for the final result file\n");
			return -8;
		}

		while( fgets(line, MaxLine, fpread) != NULL )
		{
			line_csv++;
			flag = 0;
			while( ( strlen(line)==MaxLine-1) && ( line[MaxLine-2]!='\n' ) )   
			{
				flag = 1;
				fputs( line, fplong);
				if( fgets(line, MaxLine, fpread) == NULL )
				{
					fprintf(fpbad, "csv2bin: error fgets at line %d\n", line_csv);
					return -9;
				}
			}
			if( flag)
			{
				fputs( line, fplong);
				fprintf(fplong, "illegal record line %d: too long!\n\n", line_csv);
				continue;
			}

			ichild = cnt_record % NCHILD;
			write( par[ichild][1], line, MaxLine );
			cnt_record ++;
#ifdef HSSDEBUG
	printf("write to child %d, cnt_record=%d\n", ichild, cnt_record);
#endif

			if( ichild == NCHILD-1 )   //one round send finished; time to get results back
			{
				for( i=0; i<NCHILD; i++)
				{
					read( chd[i][0], &flag, sizeof(int) );
#ifdef HSSDEBUG
	printf("read from child %d\n", i);
#endif
					if(flag)
					{
						read(chd[i][0], &stu, sizeof(stu) );
						read(chd[i][0], stuname, sizeof(uint8_t)*stu.lenName );
						if( fwrite( &stu, sizeof(stu), 1, fpwrite)!=1 )
						{
							fprintf(fpbad, "csv2bin: error fwrite stu\n");
							return -10;
						}
						else if( fwrite( stuname, sizeof(uint8_t), stu.lenName, fpwrite)!=stu.lenName )
						{
							fprintf(fpbad, "csv2bin: error fwrite stuname\n");
							return -11;
						}
						else
							cnt_succeed += 1;
					}
				}
			}  // end of if ichild == NCHILD - 1

		}// end of while fgets

		if( ferror(fpread) )	
			fprintf(fpbad, "error occur when fgets %s, never reach end of file\n\n", filecsv);

		if( ichild != NCHILD-1 )   //one round send finished; time to get results back
		{
			for( i=0; i<=ichild; i++)
			{
				read( chd[i][0], &flag, sizeof(int) );
				if(flag)
				{
					read(chd[i][0], &stu, sizeof(stu) );
					read(chd[i][0], stuname, sizeof(uint8_t)*stu.lenName );
					if( fwrite( &stu, sizeof(stu), 1, fpwrite)!=1 )
					{
						fprintf(fpbad, "csv2bin: error fwrite stu\n");
						return -10;
					}
					else if( fwrite( stuname, sizeof(uint8_t), stu.lenName, fpwrite)!=stu.lenName )
					{
						fprintf(fpbad, "csv2bin: error fwrite stuname\n");
						return -11;
					}
					else
						cnt_succeed += 1;
				}
			}
		}  // end of if ichild

		fflush(fpwrite);
		if( fseek(fpwrite, 0, SEEK_SET) == -1 )
		{
			fprintf(fpbad, "csv2bin: error fseek final cnt_succeed\n");
			return -12;
		}
		if( fwrite( &cnt_succeed, sizeof(uint32_t), 1, fpwrite) != 1 )
		{
			fprintf(fpbad, "csv2bin: error fwrite final cnt_succed\n");
			return -13;
		}

		free(longrecords);
		free(badrecords);

		printf("\nTRANSFORM FINISHED!!!!\n\n");
		return 0;
	}// end of parent


	else   //////////////////////// child: /////////////////////
	{
#ifdef HSSDEBUG
			FILE * real_stdout = fopen("/dev/tty", "w");
			setbuf(real_stdout, NULL);
			fprintf( real_stdout, "this is child %d\n", getpid() );
#endif
		while( read( STDIN_FILENO, line, MaxLine ) > 0 )  // or while(1) ??????????
		{
#ifdef HSSDEBUG
			childcnt++;
			fprintf(real_stdout, "pid= %d, childcnt= %d\n", getpid(), childcnt);
#endif
			flag = line_csv2bin( line, &stu, stuname, fpbad);
			write(STDOUT_FILENO, &flag, sizeof(int) );
			if( flag > 0 )
			{
				write(STDOUT_FILENO, &stu, sizeof(stu) );
				write(STDOUT_FILENO, stuname, sizeof(uint8_t)*stu.lenName );
			}
		}
		return 0;
	}

}
