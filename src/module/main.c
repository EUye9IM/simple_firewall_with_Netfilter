#include "main.h"
#include "rules_list.h"

#include <linux/ip.h>
#include <linux/module.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/proc_fs.h>
#include <linux/stddef.h>
#include <linux/tcp.h>
#include <linux/udp.h>
// #include <net/bonding.h>

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("A simple personal firewall");

// gloable
struct List list_head;
// netfilter hook
struct nf_hook_ops nfho_local_in;
// proc
struct proc_dir_entry *proc_file;
struct file_operations fopt;

// hook
unsigned hookLocalIn(void *priv, struct sk_buff *skb,
					 const struct nf_hook_state *state) {
	struct iphdr *iph;
	struct tcphdr *tcph = NULL;
	struct udphdr *udph = NULL;
	struct List *p;

	unsigned long int ip;
	unsigned short port;
	iph = ip_hdr(skb);
	ip = iph->saddr;
	port = 0;
	switch (iph->protocol) { //选择协议类型
	case IPPROTO_TCP:
		tcph = tcp_hdr(skb); //获得tcp头
		port = tcph->dest;
	case IPPROTO_UDP:
		udph = udp_hdr(skb); //获得udp头
		port = udph->dest;
		break;
	default:
		break;
	}
	p = list_head.next;
	while (p != NULL) {
		switch (p->rule.rule_type) {
		case RULE_BANIP:
			if (p->rule.data == ip)
				return NF_DROP;
			break;
		case RULE_BANPORT:
			if (p->rule.data == port)
				return NF_DROP;
			break;
		default:
			break;
		}
		p = p->next;
	}

	return NF_ACCEPT;
}
void hook_init(void) {
	// set hook
	nfho_local_in.hook = hookLocalIn;
	nfho_local_in.pf = PF_INET;
	nfho_local_in.priority = NF_IP_PRI_FIRST;
	nf_register_net_hook(&init_net, &nfho_local_in);
}
void hook_exit(void) {
	// remove hook
	nf_unregister_net_hook(&init_net, &nfho_local_in);
}
// proc
static ssize_t proc_write(struct file *file, const char __user *ubuf,
						  size_t count, loff_t *ppos) {
	const int BUFSIZE = 10240;
	int c;
	int num;
	long unsigned int i1, i2;
	struct List *p;
	struct Rule rule;
	char buf[BUFSIZE];
	buf[BUFSIZE - 1] = 0;
	if (*ppos > 0 || count > BUFSIZE)
		return -EFAULT;
	if (copy_from_user(buf, ubuf, count))
		return -EFAULT;

	p = &list_head;
	c = strlen(buf);
	num = sscanf(buf, "%lu %lu", &i1, &i2);
	if (num == 2) {
		switch (i1) {
		case FIREWALL_RULE_CLEAR:
			clearList(&list_head);
			break;
		case FIREWALL_RULE_ADD_IP:
			rule.rule_type = RULE_BANIP;
			rule.data = (unsigned long int)i2;
			addList(&list_head, rule);
			break;
		case FIREWALL_RULE_ADD_PORT:
			rule.rule_type = RULE_BANPORT;
			rule.data = (unsigned short int)i2;
			addList(&list_head, rule);
			break;
		case FIREWALL_RULE_REMOVE_IP:
			rule.rule_type = RULE_BANIP;
			rule.data = (unsigned long int)i2;
			removeList(&list_head, rule);
			break;
		case FIREWALL_RULE_REMOVE_PORT:
			rule.rule_type = RULE_BANPORT;
			rule.data = (unsigned short int)i2;
			removeList(&list_head, rule);
			break;
		default:
			break;
		}
	}
	*ppos = c;
	return c;
}
static ssize_t proc_read(struct file *file, char __user *ubuf, size_t count,
						 loff_t *ppos) {
	const int BUFSIZE = 10240;
	struct List *p;
	char buf[BUFSIZE];
	int len = 0, t;
	if (*ppos > 0 || count < BUFSIZE)
		return 0;

	p = list_head.next;
	while (p != NULL) {
		t = len;
		len += sprintf(buf + t, "%u %lu\n", (unsigned int)p->rule.rule_type,
					   p->rule.data);
		if (len >= BUFSIZE - 16)
			break;
		p = p->next;
	}

	if (copy_to_user(ubuf, buf, len))
		return -EFAULT;
	*ppos = len;
	return len;
}
void proc_init(void) {
	// make file
	fopt.owner = THIS_MODULE;
	fopt.read = proc_read;
	fopt.write = proc_write;
	proc_file = proc_create(MOD_NAME, 0644, NULL, &fopt);
}
void proc_exit(void) {
	// remove file
	remove_proc_entry(MOD_NAME, NULL);
}
// main
static __init int personalFirewallInit(void) {
	pr_info("personal_firewall install\n");
	// hook
	hook_init();
	// proc
	proc_init();
	// inititial list
	list_head.next = NULL;

	return 0;
}
module_init(personalFirewallInit);

static __exit void personalFirewallExit(void) {
	pr_info("personal_filrewall remove\n");
	// hook
	hook_exit();
	// proc
	proc_exit();
	// clear list
	clearList(&list_head);
	return;
}
module_exit(personalFirewallExit);