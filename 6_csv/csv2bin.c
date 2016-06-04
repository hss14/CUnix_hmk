#include "csv6.h"

int csv2bin( const char *filecsv, const char *filebin )
{
	FILE *fpcsv, *fpbin, *fpbad;
	char *badrecords;
	char line[MaxLine];
	struct StuRecord stu;
	uint8_t stuname[MaxName];
	uint32_t cnt_record=0, line_csv=1;
	int cnt_char;    // pointer for line[]
	int name_start;  //mark position of name 
	int flag_toolong = 0;
	int flag_wrongid = 0;
	int i;
	char c;

	if( (fpcsv = fopen(filecsv, "r")) == NULL )
	{
		fprintf(stderr, "csv2bin: error fopen %s\n", filecsv );
		return -1;
	}
	if( (fpbin = fopen(filebin, "wb")) == NULL )
	{
		fprintf(stderr, "csv2bin: error fopen %s\n", filebin );
		return -2;
	}
	if( fwrite( &cnt_record, sizeof(uint32_t), 1, fpbin)!=1 )
	{
		fprintf(stderr, "csv2bin: error fwrite cnt_record at the beginning, exit..\n");
		return -3;
	}

	badrecords = malloc( strlen(filecsv)+5 );
	strcpy(badrecords, filecsv);
	strcat(badrecords, ".bad");
	if( ( fpbad = fopen(badrecords, "w") ) == NULL )
		fprintf(stderr, "csv2bin: error fopen create %s, yet continue without record bad records\n", badrecords);

	// discard the first line
	if( fgets(line, MaxLine, fpcsv) == NULL )
	{
		fprintf(stderr, "csv2bin: error fgets at the the first line\n");
		return -4;
	}
	// if real length of line > MaxLine: dicard this line;   always check and discard each time fgets
	while( ( strlen(line)==MaxLine-1) && ( line[MaxLine-2]!='\n' ) )   
	{
		flag_toolong = 1;
		fprintf(stderr, "TOO LONG the first line!\n");
		fputs( line, fpbad);
		if( fgets(line, MaxLine, fpcsv) == NULL )
		{
			fprintf(stderr, "csv2bin: error fgets at the first line\n");
			return -5;
		}
	}
	if( flag_toolong)
	{
		fputs( line, fpbad);
		fputc( '\n', fpbad);
	}
	
	while( fgets(line, MaxLine, fpcsv) != NULL )
	{
		line_csv++;
		flag_toolong = 0;
		flag_wrongid = 0;
		while( ( strlen(line)==MaxLine-1) && ( line[MaxLine-2]!='\n' ) )   
		{
			flag_toolong = 1;
			fprintf(stderr, "illegal record line %d: too long!\n", line_csv);
			fputs( line, fpbad);
			if( fgets(line, MaxLine, fpcsv) == NULL )
			{
				fprintf(stderr, "csv2bin: error fgets at line %d\n", line_csv);
				return -6;
			}
		}
		if( flag_toolong)
		{
			fputs( line, fpbad);
			fputc( '\n', fpbad);
			continue;
		}
		
		// student id;  line[0~9]
		for( cnt_char=0; cnt_char<10; cnt_char+=2)
			stu.stuid[cnt_char/2] = convert( line[cnt_char], line[cnt_char+1] );
		for( i=0; i<5; i++)
			if( stu.stuid[i] == 0xFF)
				flag_wrongid = 1;
		if( flag_wrongid)
		{
			fprintf(stderr, "illegal record line %d: illegal id segment!\n", line_csv);
			fputs( line, fpbad );
			fputc( '\n', fpbad );
			continue;
		}
				
		
		// allow ' 'around ',' as separator
		while( ( ( line[cnt_char] == ',' ) || ( line[cnt_char] == ' ' ) ) && ( cnt_char < MaxLine ) )
			cnt_char ++;
		if( cnt_char == MaxLine ) 
			//discard any incomplete record and log in file bad record; 
			//always do the check and discard and log thing in the following 
		{
			fprintf(stderr, "illegal record %d: no name segment found!\n", line_csv);
			fputs( line, fpbad );
			fputc( '\n', fpbad );
			continue;
		}

		// name;  lenName;
		name_start = cnt_char;
		while( ( line[cnt_char] != ',' ) && (cnt_char < MaxLine ) )
			cnt_char ++;
		if( cnt_char == MaxLine )
		{
			fprintf(stderr, "illegal record %d: name segment incomplete!\n", line_csv);
			fputs( line, fpbad );
			fputc( '\n', fpbad );
			continue;
		}
		if( cnt_char - name_start + 1 > MaxName)
		{
			stu.lenName = MaxName;
			fputs( line, fpbad );
			fputc( '\n', fpbad );
			fprintf(stderr, "illegal csv format line %d:name too long, truncate to 254 characters\n", line_csv);
		}
		else stu.lenName = cnt_char - name_start + 1;

		i=0;
		while( ( (c=line[name_start])!=',' ) && ( i < stu.lenName-1 ) )
		{
			stuname[i] = (uint8_t)c;
			i++;   
			name_start ++;
		}
		stuname[i] = '\0';

		//allow ' ' after ',' as separator
		while(  ( (line[cnt_char]==',') || (line[cnt_char]==' ') ) && (cnt_char<MaxLine)  )
			cnt_char ++;
		if( cnt_char == MaxLine )
		{ 
			fprintf(stderr, "illegal record line %d: nothing after name segment found!\n", line_csv);
			fputs( line, fpbad );
			fputc( '\n', fpbad );
			continue;
		}

		// department;   dpt[0~8]
		i=0;
		c=line[cnt_char];
		while( (c!=',') && (c!=' ') && (i<8) && (cnt_char < MaxLine) )
		{
			stu.dpt[i] = (uint8_t)c;
			i++;
			cnt_char++;
			c = line[cnt_char];
		}
		if( cnt_char == MaxLine )
		{
			fprintf(stderr, "illegal record %d: nothing after department segment found!\n", line_csv );
			fputs( line, fpbad);
			fputc( '\n', fpbad );
			continue;
		}
		stu.dpt[i] = '\0';
		//if department > 8 chars, discard the rest, notify illegal abbreviation
		if( (i==8) && (c!=' ') && (c!=',') )
		{
			fprintf(stderr, "illegal csv format line %d: Department longer than 8 characters, truncate\n", line_csv);
			fputs( line, fpbad);
			fputc( '\n', fpbad );
			while((line[cnt_char]!=' ') && (line[cnt_char]!=',') && (cnt_char < MaxLine) )
				cnt_char++;
			if( cnt_char == MaxLine )
			{
				fputs( line, fpbad);
				fputc( '\n', fpbad );
				continue;
			}
		}

		//disgard separator, or any other illegal characters for the age segment
		while( ( (line[cnt_char]<'0')||(line[cnt_char]>'9') ) && (cnt_char<MaxLine) && (line[cnt_char]!='\n')  )
			cnt_char ++;
		if( ( cnt_char == MaxLine ) || (line[cnt_char]=='\n') )
		{
			fprintf(stderr, "illegal record line %d, no age segment found!\n", line_csv);
			fputs( line, fpbad);
			fputc( '\n', fpbad );
			continue;
		}
		
		//age;
		if( (cnt_char < MaxLine-1) && (line[cnt_char+1]>='0') && (line[cnt_char+1]<='9') )
			stu.age = convert( line[cnt_char], line[cnt_char+1] );  // 2 digits age
		else
			stu.age = convert( '0', line[cnt_char] );   		// 1 digit age

		// write to file bin;
		if( fwrite( &stu, sizeof(stu), 1, fpbin ) != 1 )
			fprintf( stderr, "error csv2bin: fwrite stu for record %s\n", line );
		else if( fwrite( stuname, sizeof(uint8_t), stu.lenName, fpbin ) != stu.lenName )
			fprintf( stderr, "error csv2bin: fwrite name for record %s\n", line );
		else	
			cnt_record++;

	}

	fflush(fpbin);
	if( fseek(fpbin, 0, SEEK_SET) == -1 )
	{
		fprintf(stderr, "csv2bin: error fseek\n");
		return -7;
	}
	if( fwrite( &cnt_record, sizeof(uint32_t), 1, fpbin) != 1 )
	{
		fprintf(stderr, "csv2bin: error fwrite cnt_record\n");
		return -8;
	}

	if( ferror(fpcsv) )	
	{
		fprintf(stderr, "error occur when fgets %s, never reach end of file\n", filecsv);
		fputs("\nreading didn't reach end of csv file\n", fpbad);
	}

	free(badrecords);
	return 0;
}
