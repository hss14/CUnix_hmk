#ifndef __BINARY_TREE__
#define __BINARY_TREE__

#include "linklist.h" 

struct binary_tree_node {
	struct binary_tree_node* left;
	struct binary_tree_node* right;
};

#define BINARY_TREE_INIT(name) { NULL }

#define BINARY_TREE_HEAD(name) struct binary_tree_node name = BINARY_TREE_INIT(name) 

int insert_node( struct binary_tree_node* where);

#define binary_tree_traverse()

#define binary_tree_destroy(head, type, member) do {	\
		\
} while(0)


#endif
