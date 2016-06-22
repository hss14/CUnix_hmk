#include "cup.h"

int main()
{
	struct cup *pcup;
	struct cup *ncup;
	struct queue myqueue;
	struct record rec;

	rec.capacity = CAPACITY_INIT;
	rec.len = 0;
	if( (rec.prev = malloc( sizeof(int *) * rec.capacity ) ) == NULL )
	{
		perror("main: initialize struct record rec: malloc");
		exit(EXIT_FAILURE);
	}

	pcup = newcupnode( ABC_INIT, 0, 0, NULL);
	newqueue( &myqueue );
	inqueue( pcup, &myqueue );

	if( outqueue( &ncup, &myqueue ) == -1 )
	{
		printf("empty queue! no solution found!\n");
		exit(0);
	}
	while( ncup->abc != ABC_GOAL )
	{
		push_children( ncup, &rec, &myqueue ); 
		if( outqueue( &ncup, &myqueue ) == -1 )
		{
			printf("empty queue! no solution found!\n");
			exit(0);
		}	
	} 	

	pr_result( ncup );
	free(rec.prev);
	destroy_queue( &myqueue );

	exit(0);
}

