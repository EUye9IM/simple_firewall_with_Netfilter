#include "main.h"
#include "rules_list.h"
#include <linux/slab.h> //kfree kmalloc
#include <linux/stddef.h>

void clearList(struct List *list_head) {
	struct List *p, *q;
	if (list_head == NULL)
		return;
	p = list_head->next;
	while (p != NULL) {
		q = p;
		p = p->next;
		kfree(q);
	}
	list_head->next = NULL;
}
void addList(struct List *list_head, struct Rule rule) {
	struct List *p, *new;
	p = list_head->next;
	if (list_head == NULL)
		return;
	new = kmalloc(sizeof(struct List), 0);
	if (new == NULL)
		return;
	new->rule = rule;
	new->next = p;
	list_head->next = new;
	return;
}
void removeList(struct List *list_head, struct Rule rule) {
	struct List *p, *q;
	if (list_head == NULL)
		return;
	p = list_head;
	while (p->next != NULL) {
		if (p->next->rule.data == rule.data &&
			p->next->rule.rule_type == rule.rule_type) {
			q = p->next;
			p->next = q->next;
			kfree(q);
			return;
		}
		p = p->next;
	}
	return;
}