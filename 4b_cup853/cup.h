#ifndef __H_CUP_HMK4__
#define __H_CUP_HMK4__

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define AROOM 8
#define BROOM 5
#define CROOM 3
#define ABC_INIT 800
#define ABC_GOAL 440
#define CAPACITY_INIT 64


struct cup
{
	int abc;   	// 100a+10b+c
	int opt;  	// how to get to this point from its parent, corresponding to optstring[opt]
	int step;     	// count of step
	struct cup *parent;
};

struct queue
{
	int capacity;           // room[ 0 ~ capacity-1 ];
	int head;		// room[head] is the first in queue
	int tail;		// room[tail] is the last in queue
	struct cup **room;	// elements are address of each struct cup node ever generated
};

struct record   //to record combination that has already showed up, lower the number of nodes to traverse
{
	int capacity;   // length of prev[]; realloc if not enough
	int len;	// number of elements now stored in prev[]
	int *prev;	// array of "abc"
};





/* --------------------------------------- cupnode.c ------------------------------------------------ */
struct cup *newcupnode(int abc, int step, int opt, struct cup *parent);		// return a newly malloced struct cup* node

void pr_result( struct cup *final);			// look back upon to the root, print out the operation path

void push_children( struct cup *pparent, struct record *rec, struct queue *myqueue);
// use function "getnext" to get all the possible next states, then use function "check_record" to rule out already existed states
// construct corresponding child struct cup *nodes and push into the queue





/* --------------------------------------- record.c ------------------------------------------------ */
int check_record( struct record *rec, int abc);
// deal with struct record
// if abc already exits in rec, return 0;
// otherwise, return 1, and add abc to rec





/* --------------------------------------- queue.c ------------------------------------------------ */
// deal with struct queue. function names apparent enough about the operation of the queue
void newqueue( struct queue *myqueue ); 			// initialize the queue
void inqueue(struct cup *member, struct queue *myqueue);        // push into queue
int outqueue(struct cup **member, struct queue *myqueue );      // pop from queue
void destroy_queue(struct queue *myqueue);			// free all elements and then free itself





/* --------------------------------------- getnext.c ------------------------------------------------ */
void getnext( int abc, int nextstep[] );   
// load the child states of "abc" into "nextstep[0~5]={initialized 0}, corresponding to operation in optsring[6]
// all possible next states are loaded, including states who have showed up before 

int get_abc(int a, int b, int c);  	// transform a,b,c -> abc
int mygeta(int abc);			// transform: abc -> a
int mygetb(int abc);			// transform: abc -> b
int mygetc(int abc);			// transform: abc -> c

int fromcup(int a, int b, int broom);     // after a pour into b, what remains in a
int tocup(int a, int b, int broom);       // after a pour into b, what remains in b





#ifdef HSSDEBUG
void pr_cupnode(struct cup *node);
#endif




#endif
