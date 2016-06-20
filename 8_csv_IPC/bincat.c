#include "csv8.h"

int bincat(const char *filebin)
{
	int fd;
	struct stat sbuf;
	void *src;
	struct StuRecord stu;
	uint8_t stuname[MaxName];
	uint32_t cnt;
	int i,j;

	if( ( fd = open(filebin, O_RDONLY ) ) == -1 )
	{
		fprintf(stderr, "bincat: error open %s\n", filebin );
		return -1;
	}
	if( fstat(fd, &sbuf) < 0 )
	{
		fprintf(stderr, "bincat: error fstat %s\n", filebin );
		return -2;
	}

	if( (src = mmap(0, sbuf.st_size, PROT_READ, MAP_SHARED, fd, 0) ) == MAP_FAILED )
	{
		fprintf(stderr, "bincat: error mmap %s\n", filebin );
		return -3;
	}
	memcpy(&cnt, src, sizeof(uint32_t) );
	src += sizeof(uint32_t);

	fprintf(stdout, "\n===================================================\nsource: %s\nnumber of records: %d\n", filebin, cnt );
	fprintf(stdout, "================  student records =================\n" );
	fprintf(stdout, "Student ID | Department | age | name\n");

	for( i=0; i<cnt; i++)
	{
		memcpy(&stu, src, sizeof(struct StuRecord) );
		src += sizeof(struct StuRecord);
		memcpy(stuname, src, stu.lenName);
		src += stu.lenName;

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
