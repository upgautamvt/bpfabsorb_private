#include <linux/bpf.h>
#include <linux/if_ether.h>
#include <linux/ip.h>
#include <linux/types.h>
#include <bpf/bpf_helpers.h>

struct {
    __uint(type, BPF_MAP_TYPE_ARRAY);
    __type(key, __u32);
    __type(value, __be32); // Changed to __be32 for IP addresses
    __uint(max_entries, 1024);
    __uint(pinning, LIBBPF_PIN_BY_NAME);
} my_map SEC(".maps");


SEC("tc/ingress")
int verybig(struct __sk_buff *skb) {
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

    //do something time consuming
    bpf_printk("From BPF: Empty output\n");
    bpf_printk("From BPF: Empty output\n");
    bpf_printk("From BPF: Empty output\n");
    bpf_printk("From BPF: Empty output\n");
    bpf_printk("From BPF: Empty output\n");
    bpf_printk("From BPF: Empty output\n");
    bpf_printk("From BPF: Empty output\n");
    bpf_printk("From BPF: Empty output\n");
    bpf_printk("From BPF: Empty output\n");
    bpf_printk("From BPF: Empty output\n");

    return BPF_OK;
}

char _license[] SEC("license") = "GPL";

