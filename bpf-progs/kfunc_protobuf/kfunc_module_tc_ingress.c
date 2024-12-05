#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/bpf.h>
#include <linux/btf.h>
#include <linux/btf_ids.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/ip.h>
#include <linux/if_ether.h>
#include <linux/filter.h>
#include <linux/udp.h>

// pbtools stuff
#include "pbtools.c"

#include "dropme.c"


/* Declare the kfunc prototype */
__bpf_kfunc int kfunc_tc_ingress_clsact_filter(struct sk_buff *skb, u32 skb__sz);

/* Begin kfunc definitions */
__bpf_kfunc_start_defs();

/* Define the bpf_strstr kfunc */
__bpf_kfunc int kfunc_tc_ingress_clsact_filter(struct sk_buff *skb, u32 skb__sz)
{
    void *data = (void *)(long)skb->data;
    //void *data_end = (void *)(long)(skb->data + skb->len);

    u8 workspace[64];
    struct dropme_foo_t *foo_p;
    void * payload = data + sizeof(struct ethhdr) + sizeof(struct iphdr) + sizeof(struct udphdr);
    u32 len = *(u32 *)payload;
    printk(KERN_INFO "len is %u\n", len);
    
    // turn encoded into data of the packet
    foo_p = dropme_foo_new(&workspace[0], sizeof(workspace));
    len = dropme_foo_decode(foo_p, data + sizeof(struct ethhdr) + sizeof(struct iphdr) + sizeof(struct udphdr) + sizeof(u32), len);

    printk(KERN_INFO "drop is %d\n", foo_p->drop);
    if (foo_p->drop == 1) {
        return -1;
    }



    //// Check for invalid Ethernet header and drop the packet
    //if (data + sizeof(struct ethhdr) > data_end) {
    //    return -1; // Drop packet
    //}

    //struct ethhdr *eth = data;

    //// If not IPv4, continue processing
    //if (eth->h_proto != __constant_htons(ETH_P_IP)) {
    //    return 0; // Continue processing
    //}

    //// Check for invalid IP header
    //if (data + sizeof(struct ethhdr) + sizeof(struct iphdr) > data_end) {
    //    return -1; // Drop packet
    //}

    //struct iphdr *ip = (struct iphdr *)(data + sizeof(struct ethhdr));
    //__be32 src_ip = ip->saddr; // Get source IP

    //// Check if source IP matches the value in the map
    //if (src_ip == *value) {
    //    return -1; // Drop packets here and return
    //}

    return 0; // Accept packet
}

/* End kfunc definitions */
__bpf_kfunc_end_defs();

/* Define the BTF kfuncs ID set */
BTF_KFUNCS_START(bpf_kfunc_example_ids_set)
BTF_ID_FLAGS(func, kfunc_tc_ingress_clsact_filter)
BTF_KFUNCS_END(bpf_kfunc_example_ids_set)

/* Register the kfunc ID set */
static const struct btf_kfunc_id_set bpf_kfunc_example_set = {
   .owner = THIS_MODULE,
   .set = &bpf_kfunc_example_ids_set,
};

/* Function executed when the module is loaded */
static int __init kfunc_module_tc_ingress_init(void)
{
   int ret;

   printk(KERN_INFO "Hello, world!\n");
   /* Register the BTF kfunc ID set for BPF_PROG_TYPE_KPROBE */
   ret = register_btf_kfunc_id_set(BPF_PROG_TYPE_SCHED_CLS, &bpf_kfunc_example_set);
   if (ret)
   {
       pr_err("bpf_kfunc_example: Failed to register BTF kfunc ID set\n");
       return ret;
   }
   printk(KERN_INFO "bpf_kfunc_example: Module loaded successfully\n");
   return 0; // Return 0 if successful
}

/* Function executed when the module is removed */
static void __exit kfunc_module_tc_ingress_exit(void)
{
   /* Unregister the BTF kfunc ID set */
   // unregister_btf_kfunc_id_set(BPF_PROG_TYPE_KPROBE, &bpf_kfunc_example_set);
   printk(KERN_INFO "Goodbye, world!\n");
}

/* Macros to define the module’s init and exit points */
module_init(kfunc_module_tc_ingress_init);
module_exit(kfunc_module_tc_ingress_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Uddhav P. Gautam");
MODULE_DESCRIPTION("Module for tc_ingress_clsact_filter functionality accessible via BPF kfunc");
MODULE_VERSION("1.0");
