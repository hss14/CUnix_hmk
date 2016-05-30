#include <stdio.h>
#include <stdlib.h>

int insert_node( struct binary_tree_node* where)
{
	if (where != NULL)
	{
		printf("error: %s :%d: try to insert a node to some occupied tree node!\n", __FILE__,__LINE__);
		return -1;
	}
	struct binary_tree_node* node = malloc( sizeof(struct binary_tree_node) );
	node->left = NULL;
	node->right = NULL;
	where = node;
	return 0;
}


