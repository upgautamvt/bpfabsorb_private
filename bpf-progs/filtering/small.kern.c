#include <linux/bpf.h>
#include <linux/if_ether.h>
#include <linux/ip.h>
#include <linux/types.h>
#include <bpf/bpf_helpers.h>

SEC("tc/ingress")
int small(struct __sk_buff *skb) {
    return BPF_OK; //ACCEPT packet
}

char _license[] SEC("license") = "GPL";

