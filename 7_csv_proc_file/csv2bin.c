#include "csv7.h"

int csv2bin( const char *filecsv, const char *filebin )
{
	const char *dir = DirectoryTmp;
	FILE *fpread, *fpwrite, *fpbad, *fplong;
	char *csvsplit, *binsplit, *badrecords, *longrecords;
	char line[MaxLine];
	uint32_t cnt_record=0, cnt_succeed = 0, line_csv=1, child_cnt;
	int flag_toolong = 0;
	uint32_t index = 1;
	pid_t pid, pidreturn;
	struct pidlist *childlist = NULL;
	int status, rstat;
	long int offset[2];
	uint8_t *block = NULL;

#ifdef HSSDEBUG
	setbuf(stdout,NULL);
#endif

	badrecords = malloc( strlen(filecsv)+5 );
	strcpy(badrecords, filecsv);
	strcat(badrecords, ".err");

	longrecords = malloc( strlen(filecsv)+6 );
	strcpy(longrecords, filecsv);
	strcat(longrecords, ".long");

	csvsplit = malloc( strlen(filecsv)+40 );
	sprintf(csvsplit, "%s%s.%d", dir, filecsv, index);

	binsplit = malloc( strlen(filebin)+40 );

	if( ( fpbad = fopen(badrecords, "a") ) == NULL )
	{
		fprintf(stdout, "csv2bin: error fopen create %s, exit because of error at the very beginning\n", badrecords);
		return -1;
	}

	if( mkdir(dir, S_IRWXU|S_IRWXG|S_IRWXO ) != 0 )
	{
		fprintf(fpbad, "error mkdir %s\n", dir);
		return -10;
	}

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
	if( (fpwrite = fopen(csvsplit, "a")) == NULL )
	{
		fprintf(fpbad, "csv2bin: error fopen %s\n", csvsplit );
		return -4;
	}

	// discard the first line
	if( fgets(line, MaxLine, fpread) == NULL )
	{
		fprintf(fpbad, "csv2bin: error fgets at the first line\n");
		return -5;
	}
	// if real length of line > MaxLine: dicard this line;   always check and discard each time fgets
	while( ( strlen(line)==MaxLine-1) && ( line[MaxLine-2]!='\n' ) )   
	{
		flag_toolong = 1;
		fputs( line, fplong);
		if( fgets(line, MaxLine, fpread) == NULL )
		{
			fprintf(fpbad, "csv2bin: error fgets at the first line\n");
			return -6;
		}
	}
	if( flag_toolong)
	{
		fputs( line, fplong);
		fprintf(fplong, "TOO LONG the first line!\n\n");
	}
	
	while( fgets(line, MaxLine, fpread) != NULL )
	{
		line_csv++;
		flag_toolong = 0;
		while( ( strlen(line)==MaxLine-1) && ( line[MaxLine-2]!='\n' ) )   
		{
			flag_toolong = 1;
			fputs( line, fplong);
			if( fgets(line, MaxLine, fpread) == NULL )
			{
				fprintf(fpbad, "csv2bin: error fgets at line %d\n", line_csv);
				return -7;
			}
		}
		if( flag_toolong)
		{
			fputs( line, fplong);
			fprintf(fplong, "illegal record line %d: too long!\n\n", line_csv);
			continue;
		}
		fflush(fplong);
// ！！！如果不fflush，fork以后缓冲区也被复制了多份，导致fplong中有多条重复数据

		fputs(line, fpwrite);
		cnt_record ++;

		if( (cnt_record % SplitSize) == 0 )   //one split csv file finished: close, fork, open new file
		{
			fclose(fpwrite);
		
			if( (pid=fork()) < 0 ) {
				fprintf(fpbad, "csv2bin: error fork at count = %d, line = %d\n", cnt_record, line_csv);
				return -8;
			} else if( pid == 0 ) {      
/////   child  ///////
#ifdef HSSDEBUG
				printf("child %d 1: pid = %d begin!\n", index, getpid() );
#endif
				csvsplit = malloc( strlen(filecsv)+40 );
				sprintf(csvsplit, "%s%s.%d", dir, filecsv, index);
				binsplit = malloc( strlen(filebin)+40 );
				sprintf(binsplit, "%s%s.%d", dir, filebin, index);
				
				if( ( status = child_csv2bin(csvsplit, binsplit, badrecords) ) < 0 )
					fprintf(fpbad, "error %d return from pid %d\n\n", status, getpid() );
				free(csvsplit);
				free(binsplit);
#ifdef HSSDEBUG
				printf("child %d 4: just before end, pid = %d end!, status= %d\n", index, getpid(), status);
#endif
				return status;	
			} else {
/////    parent	///////
				addlist( pid, &childlist);
#ifdef HSSDEBUG
	printlist(childlist);
#endif
				index++;
				sprintf(csvsplit, "%s%s.%d", dir, filecsv, index);
				if( (fpwrite = fopen(csvsplit, "a")) == NULL )
				{
					fprintf(fpbad, "csv2bin: error fopen %s\n", csvsplit );
					return -4;
				}

			} //end of parent
		}  // end of if cnt_record % SplitSize == 0

	}// end of while fgets

#ifdef HSSDEBUG
	printf("father end fgets!\n");
	printlist(childlist);
#endif

	if( ferror(fpread) )	
		fprintf(fpbad, "error occur when fgets %s, never reach end of file\n\n", filecsv);

	fclose(fpwrite);
	fclose(fpread);
	fclose(fplong);

#ifdef HSSDEBUG
	printf("cnt_record before final check of split file= %d\n", cnt_record);
#endif

	if( cnt_record % SplitSize != 0 )
	{
			if( (pid=fork()) < 0 ) {
				fprintf(fpbad, "csv2bin: error fork at count = %d, line = %d\n", cnt_record, line_csv);
				return -8;
			} else if( pid == 0 ) {      
#ifdef HSSDEBUG
				printf("just before child index = %d pid = %d start\n", index, getpid());
#endif
				binsplit = malloc( strlen(filebin) + 40 );
				sprintf(binsplit, "%s%s.%d", dir, filebin, index);
				if( ( status = child_csv2bin(csvsplit, binsplit, badrecords) ) < 0 )
					fprintf(fpbad, "error %d return from pid %d\n\n", status, getpid() );
				free(binsplit);
#ifdef HSSDEBUG
				printf("just before child index = %d pid = %d end!, status= %d\n", index, getpid(), status);
#endif
				return status;	
			} else {
				addlist( pid, &childlist);
			} 
	}
	else
	{
		unlink(csvsplit);
	}

//注意所有用于写的bin不可以用"a"打开！最后要fseek到开头写覆盖cnt，若"a"每次写操作之前会被强制指到文件尾！
	if( (fpwrite = fopen(filebin, "wb")) == NULL )
	{
		fprintf(fpbad, "csv2bin: error fopen result bin file %s\n", filebin );
		return -2;
	}
	if( fwrite( &cnt_succeed, sizeof(uint32_t), 1, fpwrite)!=1 )
	{
		fprintf(fpbad, "csv2bin: error fwrite cnt_succeed for the final result file\n");
		return -3;
	}

	index = 0;
	while( (pid = pop(&childlist)) != 0)
	{
#ifdef HSSDEBUG
	printf("\npid=%d\n", pid);
#endif
		index++;
		if( ( pidreturn=waitpid(pid,&rstat,0) ) != pid )
			fprintf( fpbad, "waitpid error for pid %d\n", pid);
		else
		{
			if( WEXITSTATUS(rstat) == 0 )   // this child exit normally without error
			{
				sprintf(binsplit, "%s%s.%d", dir, filebin, index);
#ifdef HSSDEBUG
	printf("pid= %d, index=%d, binsplit=%s\n", pid, index, binsplit );
#endif
				if( (fpread = fopen( binsplit, "rb" ) ) == NULL )
				{
					fprintf( fpbad, "error open split bin file %s\n\n", binsplit);
					continue;
				}
				if( fread(&child_cnt, sizeof(uint32_t), 1, fpread) != 1 )
				{
					fprintf( fpbad, "error get count of records of file %s\n\n", binsplit);
					continue;
				}

				offset[0] = ftell(fpread);
				fseek( fpread, 0, SEEK_END);
				offset[1] = ftell(fpread);
				if( fseek( fpread, offset[0], SEEK_SET) == -1 )
				{
					fprintf( fpbad, "error fseek in %s\n\n", binsplit);
					continue;
				}
				if( ( block=realloc( block, offset[1] - offset[0] ) ) == NULL )
				{
					fprintf( fpbad, "error realloc while %s\n\n", binsplit);
					continue;
				}

				if( fread( block, sizeof(uint8_t), (offset[1]-offset[0]), fpread ) != offset[1]-offset[0] )
				{
					fprintf( fpbad, "error read info from %s\n\n", binsplit);
					continue;
				}
				if( fwrite( block, sizeof(uint8_t), (offset[1]-offset[0]), fpwrite ) != offset[1]-offset[0] )
				{
					fprintf( fpbad, "error read info from %s\n\n", binsplit);
					continue;
				}
				cnt_succeed += child_cnt;
				fclose( fpread );
			}
		}
	}
	fflush(fpwrite);

	if( fseek(fpwrite, 0, SEEK_SET) == -1 )
	{
		fprintf(fpbad, "csv2bin: error fseek final cnt_succeed\n");
		return -7;
	}
	if( fwrite( &cnt_succeed, sizeof(uint32_t), 1, fpwrite) != 1 )
	{
		fprintf(fpbad, "csv2bin: error fwrite final cnt_succed\n");
		return -8;
	}

#ifdef CLEARUP
	DIR *dp;
	struct dirent *dirp;
	if( (dp=opendir(dir) )==NULL )
		fprintf(fpbad, "error open directory %s while cleaning up\n", dir);
	else if( chdir(dir)<0 )
		fprintf(fpbad, "error chdir to %s\n", dir);
	else
	{
		while( (dirp=readdir(dp)) != NULL )
		{
			if( strcmp(dirp->d_name, ".")==0 || strcmp(dirp->d_name, "..")==0 )
				continue;
			unlink(dirp->d_name);
		}
		closedir(dp);
	}
	if( chdir("..")<0 )
		fprintf(fpbad, "error chdir to \"..\"\n" );
	else
		rmdir(dir);
#endif

	free(binsplit);
	free(csvsplit);
	free(longrecords);
	free(badrecords);
	
	printf("\nTRANSFORM SUCCEED!!!\n\n");
	return 0;
}
