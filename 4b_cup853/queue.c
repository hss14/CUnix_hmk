#include "cup.h"

void newqueue( struct queue *myqueue )
{
	myqueue->head = 0;
	myqueue->tail = -1;
	myqueue->capacity = CAPACITY_INIT;
	if( ( myqueue->room = malloc( myqueue->capacity * sizeof(struct cup *) ) ) == NULL )
	{
		perror("newqueue: malloc");
		exit(EXIT_FAILURE);
	}
	return;
}

void inqueue(struct cup *member, struct queue *myqueue)
{
	if( myqueue->tail < ( myqueue->capacity - 1 ) )
	{
		myqueue->tail ++;
		myqueue->room[ myqueue->tail] = member;
#ifdef HSSDEBUG
printf("(%d, %d)\n", myqueue->head, myqueue->tail);
#endif
		return;
	}
	else if( myqueue->tail == ( myqueue->capacity - 1) )
	{
		myqueue->capacity *= 2;
		if( ( myqueue->room = realloc( myqueue->room, myqueue->capacity * sizeof(struct cup*) ) ) == NULL )
		{
			perror("inqueue: realloc");
			exit(EXIT_FAILURE);
		}
		myqueue->tail ++;
		myqueue->room[ myqueue->tail ] = member;
#ifdef HSSDEBUG
printf("(%d, %d)\n", myqueue->head, myqueue->tail);
#endif
		return;
	}
	else
	{
		fprintf( stderr, "error queue tail > capacity - 1 ! exit.\n");
		exit(0);
	}
}

int outqueue(struct cup **member, struct queue *myqueue )
{
	if( myqueue->head > myqueue->tail )   //empty queue
		return -1;
	*member = myqueue->room[myqueue->head];
	myqueue->head ++;
#ifdef HSSDEBUG
printf("(%d, %d)\n", myqueue->head, myqueue->tail);
#endif
	return 0;
}

void destroy_queue(struct queue *myqueue)
{
	int i;
#ifdef HSSDEBUG
printf("destroy begins\n");
#endif
	for ( i=0; i <= myqueue->tail; i++ )
		free( myqueue->room[i] );
	free( myqueue->room );
#ifdef HSSDEBUG
printf("destroy ends\n");
#endif
}






