// return 0 if bad record; 
// else reture 1 , and load stu and stuname 


#include "csv8.h"
int line_csv2bin( const char *line, struct StuRecord *stu, uint8_t *stuname, FILE *fpbad )
{
	int flag_record = 1;
	int cnt_char;    // pointer for line[]
	int name_start;  //mark position of name 
	char c;
	int i;

	// student id;  line[0~9]
	for( cnt_char=0; cnt_char<10; cnt_char+=2)
		stu->stuid[cnt_char/2] = convert( line[cnt_char], line[cnt_char+1] );
	for( i=0; i<5; i++)
		if( stu->stuid[i] == 0xFF)
			flag_record = 0;
	if( ! flag_record )
	{
		fprintf(fpbad, "illegal record id segment!\n%s\n", line );
		fflush(fpbad);
		return 0;
	}
			
	// allow ' 'around ',' as separator
	while( ( ( line[cnt_char] == ',' ) || ( line[cnt_char] == ' ' ) ) && ( cnt_char < MaxLine ) )
		cnt_char ++;
	if( cnt_char == MaxLine ) 
		//discard any incomplete record and log in file bad record; 
		//always do the check and discard and log thing in the following 
	{
		fprintf(fpbad, "illegal record: no name segment found!\n%s\n", line );
		fflush(fpbad);
		return 0;
	}

	// name;  lenName;
	name_start = cnt_char;
	while( ( line[cnt_char] != ',' ) && (cnt_char < MaxLine ) )
		cnt_char ++;
	if( cnt_char == MaxLine )
	{
		fprintf(fpbad, "illegal record: name segment incomplete!\n%s\n", line);
		fflush(fpbad);
		return 0;
	}
	if( cnt_char - name_start + 1 > MaxName)
	{
		stu->lenName = MaxName;
		fprintf(fpbad, "illegal csv format: name too long, truncate to 254 characters\n%s\n", line);
		fflush(fpbad);
	}
	else stu->lenName = cnt_char - name_start + 1;

	i=0;
	while( ( (c=line[name_start])!=',' ) && ( i < stu->lenName-1 ) )
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
		fflush(fpbad);
		return 0;
	}

	// department;   dpt[0~8]
	i=0;
	c=line[cnt_char];
	while( (c!=',') && (c!=' ') && (i<8) && (cnt_char < MaxLine) )
	{
		stu->dpt[i] = (uint8_t)c;
		i++;
		cnt_char++;
		c = line[cnt_char];
	}
	if( cnt_char == MaxLine )
	{
		fprintf(fpbad, "illegal record: nothing after department segment found!\n%s\n", line );
		fflush(fpbad);
		return 0;
	}
	stu->dpt[i] = '\0';
	//if department > 8 chars, discard the rest, notify illegal abbreviation
	if( (i==8) && (c!=' ') && (c!=',') )
	{
		while((line[cnt_char]!=' ') && (line[cnt_char]!=',') && (cnt_char < MaxLine) )
			cnt_char++;
		if( cnt_char == MaxLine )
		{
			fprintf(fpbad, "illegal record: nothing after Department segment found!\n%s\n", line);
			fflush(fpbad);
			return 0;
		}
		fprintf(fpbad, "illegal csv format: Department longer than 8 characters, truncate\n%s\n", line);
		fflush(fpbad);
	}

	//disgard separator, or any other illegal characters for the age segment
	while( ( (line[cnt_char]<'0')||(line[cnt_char]>'9') ) && (cnt_char<MaxLine) && (line[cnt_char]!='\n')  )
		cnt_char ++;
	if( ( cnt_char == MaxLine ) || (line[cnt_char]=='\n') )
	{
		fprintf(fpbad, "illegal record: no age segment found!\n%s\n", line);
		fflush(fpbad);
		return 0;
	}

	//age;
	if( (cnt_char < MaxLine-1) && (line[cnt_char+1]>='0') && (line[cnt_char+1]<='9') )
		stu->age = convert( line[cnt_char], line[cnt_char+1] );  // 2 digits age
	else
		stu->age = convert( '0', line[cnt_char] );   		// 1 digit age


	return 1;	
}
