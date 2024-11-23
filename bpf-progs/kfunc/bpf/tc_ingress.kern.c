#include <linux/bpf.h>
#include <linux/if_ether.h>
#include <linux/ip.h>
#include <linux/types.h>
#include <bpf/bpf_helpers.h>

// Define the BPF map
struct {
    __uint(type, BPF_MAP_TYPE_ARRAY);
    __type(key, __u32);
    __type(value, __be32); // Use __be32 for IP addresses
    __uint(max_entries, 2);
    __uint(pinning, LIBBPF_PIN_BY_NAME);
} my_map SEC(".maps");

// Declare the kfunc prototype for the verifier
extern int kfunc_tc_ingress_clsact_filter(struct __sk_buff *skb, __be32 *value);

// BPF program attached to TC ingress
SEC("tc/ingress")
int verybig(struct __sk_buff *skb) {
    int index = 0;  // Initialize index
    __be32 *value = bpf_map_lookup_elem(&my_map, &index); // Lookup value from BPF map

    if (value) {
        // Call the kfunc to process the skb, passing the value from the map
        int result = kfunc_tc_ingress_clsact_filter(skb, value);

        // Logic based on kfunc return value
        if (result == 0) {
            return BPF_DROP; // DROP packet
        } else {
            return BPF_OK; // ACCEPT packet
        }
    }

    return BPF_OK; // If map lookup fails, allow packet
}

// Specify the license for the BPF program
char LICENSE[] SEC("license") = "GPL";
