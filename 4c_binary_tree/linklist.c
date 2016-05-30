#include<stdio.h>
#include<stdlib.h>

int list_add2head(struct list_head *head, struct list_head *new)
{
    if (head == NULL)
        return -1;
    new->next = head->next;
    head->next = new;
    return 0;
}

int list_add2tail(struct list_head *head, struct list_head *new)
{
	struct list_head* cur = head;
	while ( cur->next != NULL )
		cur = cur->next;
	cur->next = new;
	return 0;
}
