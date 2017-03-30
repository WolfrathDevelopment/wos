/*
 * list.c
 Joel Wolfrath 2013
 *
 * Linked list function implementations
 */

#include "list.h"

extern uint32 debug;

/* Add the node to the end of the list */

void list_add(struct w_listnode* head, struct w_listnode* node){

		struct w_listnode* it = head;

		while(it->next)
			it = NEXT_NODE(it);

		it->next = node;
}

/* Remove the node passed as an argument */

struct w_listnode* list_remove(struct w_listnode* head, struct w_listnode* node){

    struct w_listnode* del = head->next;

	/* Sanity check */

	if(del == NULL)
		return;

    if(del == node){

        head->next = del->next;
        return del;
    }


	while(NEXT_NODE(del) != node)
		del = NEXT_NODE(del);

	struct w_listnode* prev = del;
	del = prev->next;
	prev->next = del->next;

	return del;
}

