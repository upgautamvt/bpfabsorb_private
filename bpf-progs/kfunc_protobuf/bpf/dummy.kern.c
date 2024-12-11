#include <linux/bpf.h>
#include <bpf/bpf_helpers.h>


// Define the size of the __sk_buff struct (typically 256 bytes, confirm for your kernel version)
#define SKB_SIZE 256

// Declare the external kfunc prototype with __ksym
extern int kfunc_tc_ingress_clsact_filter(struct __sk_buff *skb, __u32 skb__sz) __ksym;

SEC("tc/ingress")
int kfunc_firewall(struct __sk_buff *skb) {
        // Call the kfunc to process the skb, passing the size of skb and value
    int result = kfunc_tc_ingress_clsact_filter(skb, SKB_SIZE);

    bpf_printk("Result of kfunc was %d\n", result);
        // Logic based on kfunc return value
    if (result == -1) {
         return BPF_DROP; // DROP packet
    } else {
        return BPF_OK; // ACCEPT packet
     }
 

    return BPF_OK; // If nothing in map, always allow
}

char LICENSE[] SEC("license") = "GPL";
