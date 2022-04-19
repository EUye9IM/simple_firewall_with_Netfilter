#ifndef PERSONAL_FIREWALL_RULES_LIST
#define PERSONAL_FIREWALL_RULES_LIST

// structs
#define RULE_BANIP 0x00
#define RULE_BANPORT 0x01

struct Rule{
	unsigned char rule_type;
	unsigned long int ip;
	unsigned short int port;
}

struct List{
	struct Rule rule;
	struct List *next;
}

void clearList(struct List list_head);

#endif