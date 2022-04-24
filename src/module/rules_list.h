#ifndef PERSONAL_FIREWALL_RULES_LIST
#define PERSONAL_FIREWALL_RULES_LIST

#include <linux/types.h>
#define MOD_NAME "personal_firewall"
// structs

struct Rule {
	unsigned char rule_type;
	u32 data0;
	u32 data1;
};

struct List {
	struct Rule rule;
	struct List *next;
};

void clearList(struct List *list_head);
void addList(struct List *list_head, struct Rule rule);
void removeList(struct List *list_head, struct Rule rule);
#endif