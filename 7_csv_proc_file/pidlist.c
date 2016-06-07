#include "csv7.h"

void addlist( pid_t pid, struct pidlist **head)
{
	struct pidlist *newnode, *pnode;

	newnode = malloc( sizeof(struct pidlist) );
	newnode->child = pid;
	newnode->next = NULL;

	if (*head==NULL)
		*head = newnode;
	else
	{
		pnode = *head;
		while( pnode->next != NULL)
			pnode = pnode->next;
		pnode->next = newnode;
	}
	return;
}

pid_t pop( struct pidlist **head)
{
	struct pidlist *pnode;
	pid_t result;
	if( *head == NULL)
		return (pid_t)0;
	if ( *head != NULL )
	{
		pnode = *head;
		*head = pnode->next;
		result = pnode->child;
		free(pnode);
	}
	return result;
}


void printlist( struct pidlist *head)
{
	struct pidlist *pnode;
	if(head == NULL)
	{
		printf("null list!\n");
		return;
	}	
	pnode = head;
	while (pnode != NULL )
	{
		printf("%d ->", pnode->child);
		pnode = pnode->next;
	}
	printf("\n");
	return;
}

