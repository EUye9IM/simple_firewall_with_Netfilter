#ifndef PERSONAL_FIREWALL_RULES_LIST
#define PERSONAL_FIREWALL_RULES_LIST

#define MOD_NAME "personal_firewall"

// structs
#define RULE_BANIP 0x00
#define RULE_BANPORT 0x01

struct Rule {
	unsigned char rule_type;
	unsigned long int data;
};

struct List {
	struct Rule rule;
	struct List *next;
};

void clearList(struct List *list_head);
void addList(struct List *list_head, struct Rule rule);
void removeList(struct List *list_head, struct Rule rule);
#endif