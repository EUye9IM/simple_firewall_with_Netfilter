
#include "rules_list.h"


void clearList(struct List list_head){
	struct List *p=list_head.next, *q;
	while(p!=NULL){
		q = p;
		p = p->next;
		vfree(q);
	}
}