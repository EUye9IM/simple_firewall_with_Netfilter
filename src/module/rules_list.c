#include "rules_list.h"
#include "main.h"
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
	removeList(list_head, rule);
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
		if (p->next->rule.data0 == rule.data0 &&
			p->next->rule.data1 == rule.data1 &&
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