#include "cup.h"

char *optstring[6] = { "A to B", "A to C", "B to A", "B to C", "C to A", "C to B" };

struct cup *newcupnode(int abc, int step, int opt, struct cup *parent)
{
	struct cup *node;
	if( ( node = malloc( sizeof(struct cup) ) ) == NULL )
	{
		perror("newcupnode: malloc");
		exit(EXIT_FAILURE);
	}
	node->abc = abc;
	node->step = step;
	node->opt = opt;
	node->parent = parent;
#ifdef HSSDEBUG
pr_cupnode(node);
#endif
	return node;
}

void push_children( struct cup *pparent, struct record *rec, struct queue *myqueue)
{
	int nextstep[6] = {0};
	int newabc;
	int i;
	struct cup *newnode;

	getnext( pparent->abc, nextstep );
	for ( i=0; i<6; i++ )
		if( ( newabc = nextstep[i] ) )
			if( check_record( rec, newabc ) )  // add to queue	
			{
				newnode = newcupnode( newabc, pparent->step + 1, i, pparent);
				inqueue( newnode, myqueue);
			}

}

void pr_result( struct cup *final)
{
	int step;
	struct cup *node = final;
	for( step = node->step; step > 0; step-- )
	{
		printf("step %d: %s (%d %d %d) -> (%d %d %d)\n", step, optstring[node->opt], 
			mygeta(node->parent->abc), mygetb(node->parent->abc) , mygetc(node->parent->abc),
			mygeta(node->abc), mygetb(node->abc) , mygetc(node->abc) );
		node = node->parent;
	}
	return;
}

#ifdef HSSDEBUG
void pr_cupnode(struct cup *node)
{
	printf("node %lld(%03d), step %d by %lld %s\n", node, node->abc, node->step, node->parent, optstring[node->opt] );
}
#endif
