#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/ip.h>
#include <linux/if_ether.h>
#include <linux/btf.h>
#include <linux/btf_ids.h>
#include <linux/filter.h>
#include <linux/types.h>
#include <linux/bpf.h>
#include "tc_ingress_clsact_filter.h"

// Define the function to be exported as a kfunc
int tc_ingress_clsact_filter(struct sk_buff *skb, __be32 *value) {
   void *data = (void *)(long)skb->data;
   void *data_end = (void *)(long)(skb->data + skb->len);


   // Check for invalid Ethernet header and drop the packet
   if (data + sizeof(struct ethhdr) > data_end) {
       return -1; // Drop packet
   }

   struct ethhdr *eth = data;

   // If not IPv4, continue processing
   if (eth->h_proto != __constant_htons(ETH_P_IP)) {
       return 0; // Continue processing
   }

   // Check for invalid IP header
   if (data + sizeof(struct ethhdr) + sizeof(struct iphdr) > data_end) {
       return -1; // Drop packet
   }

   struct iphdr *ip = (struct iphdr *)(data + sizeof(struct ethhdr));
   __be32 src_ip = ip->saddr; // Get source IP

   // Check if source IP matches the value in the map
   if (src_ip == *value) {
       return -1; // Drop packets here and return
   }

   printk(KERN_INFO "tc_ingress_clsact_filter executed!\n");

   return 0; // Accept packet
}


/* Disables missing prototype warnings */
__bpf_kfunc_start_defs();
// kfunc wrapper for tc_ingress_clsact_filter
__bpf_kfunc int kfunc_tc_ingress_clsact_filter(struct sk_buff *skb, __be32 *value) {
   return tc_ingress_clsact_filter(skb, value);
}
__bpf_kfunc_end_defs();

// Define the kfunc ID set
BTF_KFUNCS_START(tc_ingress_clsact_kfunc_ids_set)
BTF_ID(func, kfunc_tc_ingress_clsact_filter)  // Register the kfunc
BTF_KFUNCS_END(tc_ingress_clsact_kfunc_ids_set)

// Create the kfunc set structure to register
static const struct btf_kfunc_id_set tc_ingress_clsact_kfunc_set = {
   .owner = THIS_MODULE,
   .set   = &tc_ingress_clsact_kfunc_ids_set,
};

// Module initialization function
static int __init tc_ingress_clsact_filter_module_init(void) {
   printk(KERN_INFO "tc_ingress_clsact_filter module loaded\n");
   int ret;

   ret = register_btf_kfunc_id_set(BPF_PROG_TYPE_SCHED_CLS, &tc_ingress_clsact_kfunc_set);

   // Register the kfunc
   if (ret) {
       pr_err("Failed to register BTF kfunc ID set\n");
       return ret;
   }

   printk(KERN_INFO "Module loaded successfully\n");
   return 0; // Return 0 if successful
}


// Module exit function
static void __exit tc_ingress_clsact_filter_module_exit(void) {
   printk(KERN_INFO "tc_ingress_clsact_filter module unloaded\n");

   // No need to unregister kfuncs in the current kernel version
   // Kernel may handle cleanup automatically
}

// Register module init and exit functions
module_init(tc_ingress_clsact_filter_module_init);
module_exit(tc_ingress_clsact_filter_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Uddhav P. Gautam");
MODULE_DESCRIPTION("Module for tc_ingress_clsact_filter functionality accessible via BPF kfunc");
MODULE_VERSION("1.0");