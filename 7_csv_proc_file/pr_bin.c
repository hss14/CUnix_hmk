#include "csv7.h"

int pr_bin(const char *filebin)
{
	FILE *fp;
	struct StuRecord stu;
	uint8_t stuname[MaxName];
	uint32_t cnt;
	int i,j;

	if( ( fp = fopen(filebin, "rb") ) == NULL )
	{
		fprintf(stderr, "pr_bin: error fopen %s\n", filebin );
		return -1;
	}
	if( fread( &cnt, sizeof(uint32_t), 1, fp ) != 1 )
	{
		fprintf(stderr, "pr_bin: error fread cnt:%s\n", filebin );
		return -2;
	}
	
	fprintf(stdout, "\n===================================================\nsource: %s\nnumber of records: %d\n", filebin, cnt );
	fprintf(stdout, "================  student records =================\n" );
	fprintf(stdout, "Student ID | Department | age | name\n");

	for( i=0; i<cnt; i++)
	{
		if( fread( &stu, sizeof(stu), 1, fp ) != 1 )
		{
			fprintf(stderr, "pr_bin: error fread stu for record %d of file %s\n", i, filebin );
			break;
		}
		if( fread( &stuname, sizeof(uint8_t), stu.lenName, fp ) != stu.lenName )
		{
			fprintf(stderr, "pr_bin: error fread struct stu for record %d of file %s\n", i, filebin );
			break;
		}

		for(j=0; j<5; j++)
		{
			fprintf(stdout, "%d", reverse(stu.stuid[j], 0) );
			fprintf(stdout, "%d", reverse(stu.stuid[j], 1) );
		}
		fprintf(stdout, " | %10s |  ", stu.dpt );
		fprintf(stdout, "%d", reverse(stu.age, 0) );
		fprintf(stdout, "%d", reverse(stu.age, 1) );
		fprintf(stdout, " | %s\n", stuname );
	}

	fprintf(stdout, "================ %d records printed ===============\n\n", i);
	return 0;
}
