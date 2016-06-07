#include "csv7.h"
int child_csv2bin( const char *csvsplit, const char *binsplit, const char *badrecords)
{
	uint32_t cnt_record=0;
	FILE *fpread, *fpwrite, *fpbad;
	char line[MaxLine];
	struct StuRecord stu;
	uint8_t stuname[MaxName];
	int flag_wrongid = 0;
	int cnt_char;    // pointer for line[]
	int name_start;  //mark position of name 
	char c;
	int i;

	if( (fpbad = fopen(badrecords, "a")) == NULL )
	{
		fprintf(stdout, "child csv2bin: error fopen %s\n", badrecords );
		return -1; 
	}
	if( (fpread = fopen(csvsplit, "r")) == NULL )
	{
		fprintf(fpbad, "child csv2bin: error fopen %s\n", csvsplit );
		return -2; 
	}
	if( (fpwrite = fopen(binsplit, "wb")) == NULL )
	{
		fprintf(fpbad, "child csv2bin: error fopen %s\n", binsplit );
		return -3; 
	}

	if( fwrite( &cnt_record, sizeof(uint32_t), 1, fpwrite)!=1 )
	{
		fprintf(fpbad, "csv2bin: error fwrite cnt_record in process %d,process discardedt..\n", getpid() );
		return -4; 
	}
	while( fgets(line, MaxLine, fpread) != NULL )
	{
		flag_wrongid = 0;

		// student id;  line[0~9]
		for( cnt_char=0; cnt_char<10; cnt_char+=2)
			stu.stuid[cnt_char/2] = convert( line[cnt_char], line[cnt_char+1] );
		for( i=0; i<5; i++)
			if( stu.stuid[i] == 0xFF)
				flag_wrongid = 1;
		if( flag_wrongid)
		{
			fprintf(fpbad, "illegal record id segment!\n%s\n", line );
			continue;
		}
			
		// allow ' 'around ',' as separator
		while( ( ( line[cnt_char] == ',' ) || ( line[cnt_char] == ' ' ) ) && ( cnt_char < MaxLine ) )
			cnt_char ++;
		if( cnt_char == MaxLine ) 
			//discard any incomplete record and log in file bad record; 
			//always do the check and discard and log thing in the following 
		{
			fprintf(fpbad, "illegal record: no name segment found!\n%s\n", line );
			continue;
		}

		// name;  lenName;
		name_start = cnt_char;
		while( ( line[cnt_char] != ',' ) && (cnt_char < MaxLine ) )
			cnt_char ++;
		if( cnt_char == MaxLine )
		{
			fprintf(fpbad, "illegal record: name segment incomplete!\n%s\n", line);
			continue;
		}
		if( cnt_char - name_start + 1 > MaxName)
		{
			stu.lenName = MaxName;
			fprintf(fpbad, "illegal csv format: name too long, truncate to 254 characters\n%s\n", line);
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
			fprintf(fpbad, "illegal record: nothing after name segment found!\n%s\n", line);
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
			fprintf(fpbad, "illegal record: nothing after department segment found!\n%s\n", line );
			continue;
		}
		stu.dpt[i] = '\0';
		//if department > 8 chars, discard the rest, notify illegal abbreviation
		if( (i==8) && (c!=' ') && (c!=',') )
		{
			while((line[cnt_char]!=' ') && (line[cnt_char]!=',') && (cnt_char < MaxLine) )
				cnt_char++;
			if( cnt_char == MaxLine )
			{
				fprintf(fpbad, "illegal record: nothing after Department segment found!\n%s\n", line);
				continue;
			}
			fprintf(fpbad, "illegal csv format: Department longer than 8 characters, truncate\n%s\n", line);
		}

		//disgard separator, or any other illegal characters for the age segment
		while( ( (line[cnt_char]<'0')||(line[cnt_char]>'9') ) && (cnt_char<MaxLine) && (line[cnt_char]!='\n')  )
			cnt_char ++;
		if( ( cnt_char == MaxLine ) || (line[cnt_char]=='\n') )
		{
			fprintf(fpbad, "illegal record: no age segment found!\n%s\n", line);
			continue;
		}

		//age;
		if( (cnt_char < MaxLine-1) && (line[cnt_char+1]>='0') && (line[cnt_char+1]<='9') )
			stu.age = convert( line[cnt_char], line[cnt_char+1] );  // 2 digits age
		else
			stu.age = convert( '0', line[cnt_char] );   		// 1 digit age

		// write to file bin;
		if( fwrite( &stu, sizeof(stu), 1, fpwrite ) != 1 )
			fprintf( fpbad, "error csv2bin: fwrite stu for record %s\n", line );
		else if( fwrite( stuname, sizeof(uint8_t), stu.lenName, fpwrite ) != stu.lenName )
			fprintf( fpbad, "error csv2bin: fwrite name for record %s\n", line );
		else	
			cnt_record++;

	}

	if( ferror(fpread) )	
	{
		fprintf(fpbad, "error occur when fgets %s, never reach end of file\n\n", csvsplit);
	}

	fflush(fpwrite);
	if( fseek(fpwrite, 0, SEEK_SET) == -1 )
	{
		fprintf(fpbad, "csv2bin proc %d: error fseek\n", getpid());
		return -5;
	}	
	if( fwrite( &cnt_record, sizeof(uint32_t), 1, fpwrite) != 1 )
	{
		fprintf(fpbad, "csv2bin: error fwrite cnt_record at the end proc %d\n", getpid());
		return -6;
	}

	fclose(fpwrite);
	return 0;	
}
