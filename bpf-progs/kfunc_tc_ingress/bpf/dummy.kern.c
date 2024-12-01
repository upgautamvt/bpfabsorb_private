#include <linux/bpf.h>
#include <bpf/bpf_helpers.h>

// Define map for IP filtering
struct {
    __uint(type, BPF_MAP_TYPE_ARRAY);
    __type(key, __u32);
    __type(value, __be32); // Changed to __be32 for IP addresses
    __uint(max_entries, 2);
    __uint(pinning, LIBBPF_PIN_BY_NAME);
} my_map SEC(".maps");

// Define the size of the __sk_buff struct (typically 256 bytes, confirm for your kernel version)
#define SKB_SIZE 256

// Declare the external kfunc prototype with __ksym
extern int kfunc_tc_ingress_clsact_filter(struct __sk_buff *skb, u32 skb__sz, __be32 *value, u32 value__sz) __ksym;

SEC("tc/ingress")
int kfunc_firewall(struct __sk_buff *skb) {
    int index = 0;  // Ensure index is initialized
    __be32 *value = bpf_map_lookup_elem(&my_map, &index); // Lookup value from BPF map

    if (value) {
        // Call the kfunc to process the skb, passing the size of skb and value
        int result = kfunc_tc_ingress_clsact_filter(skb, SKB_SIZE, value, sizeof(__be32));

        // Logic based on kfunc return value
        if (result == 0) {
            return BPF_DROP; // DROP packet
        } else {
            return BPF_OK; // ACCEPT packet
        }
    }

    return BPF_OK; // If nothing in map, always allow
}

char LICENSE[] SEC("license") = "GPL";
