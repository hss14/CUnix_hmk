#ifndef __LINKLIST__
#define __LINKLIST__

struct list_head {
    struct list_head* next;
};

#define LIST_HEAD_INIT(name) { NULL }
#define LIST_HEAD(name) struct list_head name = LIST_HEAD_INIT(name)

int list_add2head(struct list_head *, struct list_head *);
int list_add2tail(struct list_head *, struct list_head *);

#define list_entry(ptr, type, member) list_container_of(ptr, type, member)

#define list_container_of(ptr, type, member) ({			\
        const typeof( ((type *)0)->member ) *__mptr = (ptr);	\
        (type *)( (char *)__mptr - list_offsetof(type,member) );})

#define list_offsetof(type, member) ((size_t) &((type *)0)->member)


#define list_remove_head(head, type, member) do{	\
	struct list_head *cur, *next;	\
	if ( (head != NULL) && (head->next == NULL) )	\
	cur = head->next;	\
	next = cur->next;	\
	type *node = list_entry(cur, type, member);	\
	free(node);	\
	cur = next;	\
} while(0)


#define list_remove_tail(head, type, member) do{	\
	struct list_head *prev, *cur;	\
	if ( (head!=NULL) && (head->next!=NULL) )	\
	{	\
		prev = head;	\
		cur = prev->next;	\
		while (cur->next !=NULL)	\
		{	\
			prev = cur;	\
			cur = cur->next;	\
		}	\
		type *node = list_entry(cur, type, member);	\
		free(node);	\
		prev->next = NULL;	\
	}	\
} while(0)



#define list_destroy(head, type, member) do{ \
    for(struct list_head *cur=head.next; cur!=NULL;) { \
        struct list_head *next = cur->next;             \
        type *node = list_entry(cur, type, member);     \
        free(node);                                     \
        cur = next;                                     \
    }                                                   \
}while(0)

#endif
