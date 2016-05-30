#ifndef __QUEUE_INT__
#define __QUEUE_INT__

#include "linklist.h"

struct queue_int {
	int data;
	struct list_head *list;
};

void queue_int_push(struct list_head *head, int a)
{
	struct queue_int *node = malloc( sizeof(struct queue_int));
	node->data = a;
	list_add2tail( head, node->list);
	return;
} 

int queue_int_pop(struct list_head *head)
{
	int a;

} 





#endif
