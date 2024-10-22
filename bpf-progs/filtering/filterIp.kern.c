#include <linux/bpf.h>
#include <linux/if_ether.h>
#include <linux/ip.h>
#include <linux/types.h>
#include <bpf/bpf_helpers.h>

struct {
    __uint(type, BPF_MAP_TYPE_ARRAY);
    __type(key, __u32);
    __type(value, __be32); // Changed to __be32 for IP addresses
    __uint(max_entries, 2);
    __uint(pinning, LIBBPF_PIN_BY_NAME);
} my_map SEC(".maps");

SEC("tc/ingress")
int prog(struct __sk_buff *skb) {
    void *data = (void *)(long)skb->data;
    void *data_end = (void *)(long)skb->data_end;

    // Check if Ethernet header is valid
    if (data + sizeof(struct ethhdr) > data_end) {
        return BPF_DROP;
    }

    struct ethhdr *eth = data;

    // Check if the protocol is IP
    if (eth->h_proto != __constant_htons(ETH_P_IP)) {
        return BPF_OK;
    }

    // Check if IP header is valid
    if (data + sizeof(struct ethhdr) + sizeof(struct iphdr) > data_end) {
        return BPF_DROP;
    }

    struct iphdr *ip = (struct iphdr *)(data + sizeof(struct ethhdr)); // Correct pointer arithmetic
    __be32 src_ip = ip->saddr; // Get source IP

    int index = 0;  // Ensure index is initialized
    __be32 *value = bpf_map_lookup_elem(&my_map, &index); // Lookup value from BPF map

    if (value) {
        // Check if source IP matches the value in the map
        if (src_ip == *value) {
            // Print src_ip and value before dropping the packet
//            bpf_printk("Dropping packet from src_ip: %d.%d.%d.%d, value: %d.%d.%d.%d\n",
//                             src_ip & 0xFF,
//                             (src_ip >> 8) & 0xFF,
//                             (src_ip >> 16) & 0xFF,
//                             (src_ip >> 24) & 0xFF,
//                             (*value >> 0) & 0xFF,
//                             (*value >> 8) & 0xFF,
//                             (*value >> 16) & 0xFF,
//                             (*value >> 24) & 0xFF);
            return BPF_OK;
        } else {
            // Print src_ip and value before accepting the packet
//            bpf_printk("Accepting packet from src_ip: %d.%d.%d.%d, value: %d.%d.%d.%d\n",
//                             src_ip & 0xFF,
//                             (src_ip >> 8) & 0xFF,
//                             (src_ip >> 16) & 0xFF,
//                             (src_ip >> 24) & 0xFF,
//                             *value & 0xFF,
//                             (*value >> 8) & 0xFF,
//                             (*value >> 16) & 0xFF,
//                             (*value >> 24) & 0xFF);
            return BPF_DROP;
        }
    }
    return BPF_OK;
}

char _license[] SEC("license") = "GPL";

