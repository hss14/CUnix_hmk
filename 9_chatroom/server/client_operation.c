#include "../chatroom.h"

void  new_client( struct cinfo **phead, int fd )
{
	struct cinfo *newnode;
	
	if( (newnode = malloc( sizeof(struct cinfo) )) == NULL )
	{
		perror("new_client: malloc");
		exit(EXIT_FAILURE);
	}
	newnode->sockfd = fd;
	newnode->nameset = 0;
	
	if( *phead == NULL )
	{
		newnode->id = 0;
		newnode->next = NULL;
	}
	else
	{
		newnode->id = (*phead)->id + 1;
		newnode->next = (*phead);
	}

	(*phead) = newnode;
}

void destroy(struct cinfo **phead)
{
	struct cinfo *node = *phead;
	while( node != NULL )
	{
		close(node->sockfd);
		*phead = node->next;
		free(node);
		node = *phead;
	}
	return;
}

int delete_client( struct cinfo **phead, int fd, FILE *logfp)
{
	struct cinfo *node = *phead;
	struct cinfo *prev_node;
	while( node != NULL) 
		if (node->sockfd != fd) 
		{
			prev_node = node;
			node = node->next;
		}
		else
			break;
	if( node == NULL )
	{
		fprintf(logfp, "\nERROR: delete_client: no fd %d found in client list, therefore cannot delete\n", fd);
		return -1; 
	}
	if( node == *phead )  //delete the first node
		*phead = node->next;
	else
		prev_node->next = node->next;
	free(node);
	return 0;
}

int search_client( const struct cinfo *head, struct cinfo **ppresult, int fd)
{
	struct cinfo *node = head;
	while (node != NULL) 
		if (node->sockfd != fd) 
			node = node->next;
		else
			break;
	if ( node == NULL )
		return -1;
	*ppresult = node;
	return 0;
}

int change_name(int fd, char *s, struct cinfo **phead, FILE *logfp)
{
	int flag = 0; //whether truncate
	int namesize = strlen(s);
	struct cinfo *node = *phead;
	
	if( namesize <= 0 )
	{
		fprintf(logfp, "\nERROR: change nickname on fd %d: no name found in message\n", fd);
		return -1;   
	}
	if( s[namesize-1] == '\n' )
		namesize--;
	if( namesize >= MAXNAME ) //truncate if name is too long
	{
		flag = 1;
		namesize = MAXNAME-1;
		fprintf(logfp, "\nWARNING: name too long, truncated on fd %d, notification sended\n", fd);
	}

	// serch for sockfd in list
	if( search_client( *phead, &node, fd) == -1 )
	{
		fprintf(logfp, "\nERROR: change nickname: search_client: no fd %d found in client list\n", fd);
		return -2; 
	}
	strncpy(node->nickname, s, namesize);
	node->nickname[namesize] = '\0';
	node->nameset = 1;
	fprintf(logfp, "\nCHANGE NICKNAME:fd %d, id %d, to name \"%s\"\n", node->sockfd, node->id, node->nickname );

	return flag;

}


