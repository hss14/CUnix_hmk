#include "freport.h"

int hashcheck( long int inode )
{
	int index = inode % 100;
	struct mynode *pnode;
	struct mynode *prev_node = NULL;
	struct mynode *newnode;

	if( hashlist[index] == NULL )
	{
		newnode = malloc( sizeof(struct mynode) );
		newnode->inode = inode;
		newnode->next = NULL;
		hashlist[index] = newnode;
		return 0;
	}
	
	pnode = hashlist[index];
	while( pnode != NULL )
	{
		if( pnode->inode == inode )
			return 1;
		else if( pnode->inode < inode )
		{
			prev_node = pnode;
			pnode = prev_node->next;
		}
		else
			break;
	}
	
	newnode = malloc( sizeof(struct mynode) );
	newnode->inode = inode;
	newnode->next = pnode ;
	if( prev_node == NULL)
		hashlist[index] = newnode;
	else
		prev_node->next = newnode;
	return 0;

}
