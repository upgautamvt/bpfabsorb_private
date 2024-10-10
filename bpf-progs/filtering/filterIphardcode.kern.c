#include <linux/bpf.h>
#include <linux/if_ether.h>
#include <linux/ip.h>
#include <linux/types.h>
#include <bpf/bpf_helpers.h>

SEC("tc/ingress")
int prog(struct __sk_buff *skb) {
    void *data = (void *)(long)skb->data;
    void *data_end = (void *)(long)skb->data_end;

    // invalid ethernet header then drop the packet
    if (data + sizeof(struct ethhdr) > data_end) {
        return BPF_DROP;
    }

    struct ethhdr *eth = data;

    // everything but IPV4 should not drop
    if (eth->h_proto != __constant_htons(ETH_P_IP)) {
        return BPF_OK;
    }

    // invalid ip header should drop
    if (data + sizeof(struct ethhdr) + sizeof(struct iphdr) > data_end) {
        return BPF_DROP;
    }

    struct iphdr *ip = (struct iphdr *)(data + sizeof(struct ethhdr)); // Correct pointer arithmetic
    __be32 src_ip = ip->saddr; // Get source IP

    // drop only for 8.8.8.8 IP packets
    if (src_ip == __constant_htonl(0x08080808)) {
        return BPF_DROP; // Drop packet if source IP is 8.8.8.8
    }

    return BPF_OK;
}

char _license[] SEC("license") = "GPL";

