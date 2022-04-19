#include "rules_list.h"
#include <linux/stddef.h>
#include <linux/slab.h> //kfree kmalloc

void clearList(struct List list_head){
	struct List *p=list_head.next, *q;
	while(p!=NULL){
		q = p;
		p = p->next;
		kfree(q);
	}
}
void addList(struct List *list_head,struct Rule rule){
	struct List *p = list_head->next, *new = kmalloc(sizeof(struct List),0);
	if(new == NULL)
		return;
	new -> rule = rule;
	new -> next = p;
	list_head->next=new;
	return;
}