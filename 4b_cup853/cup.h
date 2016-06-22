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
	int abc;
	int opt;  //how to get to this point from its parent, corresponding to optstring[opt]
	int step;     //count of step
	struct cup *parent;
};

struct queue
{
	int capacity;        // room[ 0 ~ capacity-1 ];
	int head;
	int tail;
	struct cup **room;
};

struct record   //to record combination that has already showed up, lower the number of nodes to traverse
{
	int capacity;   //
	int len;	//
	int *prev;	//abc
};


struct cup *newcupnode(int abc, int step, int opt, struct cup *parent);
void push_children( struct cup *pparent, struct record *rec, struct queue *myqueue);
void pr_result( struct cup *final);



int check_record( struct record *rec, int abc);
       // if abc already exits in rec, return 0; otherwise, return 1, and add abc to rec


void newqueue( struct queue *myqueue );
void inqueue(struct cup *member, struct queue *myqueue);
int outqueue(struct cup **member, struct queue *myqueue );
void destroy_queue(struct queue *myqueue);//not finished


void getnext( int abc, int nextstep[] );
int get_abc(int a, int b, int c);
int mygeta(int abc);
int mygetb(int abc);
int mygetc(int abc);
int fromcup(int a, int b, int broom);
int tocup(int a, int b, int broom);

#ifdef HSSDEBUG
void pr_cupnode(struct cup *node);
#endif



#endif
