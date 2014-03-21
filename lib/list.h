/*
 * list.h
 * Joel Wolfrath, 2013
 *
 * Generic linked list header
 */

#ifndef LIST_H
#define LIST_H

#include "../types.h"

struct w_listnode{
	struct w_listnode* next;
};

#define LINKED_LIST_MEMBER(x) struct w_listnode x
#define LINKED_LIST(x) struct w_listnode x

#define INIT_LIST_HEAD(x) x.next = NULL

#define FIRST_NODE(x) ((&x)->next)
#define NEXT_NODE(x) x->next

#define LIST_ENTRY(ptr, type, member) \
    ((type *)((char *)(ptr) - (unsigned long)(&((type *)0)->member)))

#define LIST_FOR_EACH(it, head) \
                    it = head; \
                    while(it = it->next)

void list_add(struct w_listnode*, struct w_listnode*);
struct w_listnode* list_remove(struct w_listnode*, struct w_listnode*);

#endif
