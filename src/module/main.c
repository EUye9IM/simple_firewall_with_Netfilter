#include <linux/module.h>
#include "rules_list.h"

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("A simple personal firewall");


//gloable
struct List list_head;//vmalloc

static __init int personalFirewallInit(void) {
	printk(KERN_INFO "personal_firewall install\n");
	list_head.next = NULL;
	return 0;
}
module_init(personalFirewallInit);

static __exit void personalFirewallExit(void) {
	printf(KERN_INFO "personal_filrewall remove\n");
	
	// clear list
	clearList(list_head);
	return;
}
module_exit(personalFirewallExit)