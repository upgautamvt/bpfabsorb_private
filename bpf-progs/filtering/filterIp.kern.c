#include <linux/bpf.h>
#include <linux/if_ether.h>
#include <linux/ip.h>
#include <linux/tcp.h>
#include <linux/types.h>
#include <bpf/bpf_helpers.h>

struct {
    __uint(type, BPF_MAP_TYPE_ARRAY);
    __type(key, __u32);
    __type(value, int);
    __uint(max_entries, 2);
    __uint(pinning, LIBBPF_PIN_BY_NAME);
} my_map SEC(".maps");

SEC("tc/ingress")
int prog(struct __sk_buff *skb) {
    void *data = (void *)(long)skb->data;
    void *data_end = (void *)(long)skb->data_end;

    if (data + sizeof(struct ethhdr) > data_end) {
        return BPF_DROP;
    }

    struct ethhdr *eth = data;
    if (eth->h_proto != __constant_htons(ETH_P_IP)) {
        return BPF_OK;
    }

    if (data + sizeof(struct ethhdr) + sizeof(struct iphdr) > data_end) {
        return BPF_DROP;
    }

    struct iphdr *ip = (struct iphdr *)(eth + 1);
    __be32 src_ip = ip->saddr;

    int index;
    __be32 *value = bpf_map_lookup_elem(&my_map, &index);
    if (value) {
        if (src_ip == *value) {
            return BPF_DROP;
        }
        else {
            return BPF_OK;
        }
    }
    return BPF_OK;
}

char _license[] SEC("license") = "GPL";

