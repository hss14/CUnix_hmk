#include "cup.h"

// if abc already exits in rec, return 0
// otherwise, return 1, and add abc to rec
int check_record( struct record *rec, int abc)
{
	int i;
	for( i=0; i < rec->len; i++ )
		if( rec->prev[i] == abc )
			return 0;
	if( rec->len < rec->capacity )
	{
		rec->prev[ rec->len ] = abc;	
		rec->len ++;
		return 1;
	}
	else if( rec->len == rec->capacity )
	{
		rec->capacity *= 2;
		if( ( rec->prev = realloc( rec->prev, rec->capacity ) ) == NULL )
		{
			perror("check_record: realloc");
			exit(EXIT_FAILURE);
		}
		rec->prev[ rec->len ] = abc;
		rec->len ++;
		return 1;
	}
	else  //illegal!!!
	{
		printf("unbelievable. struct record len > capacity. exit.\n");
		exit(0);
	}
}
