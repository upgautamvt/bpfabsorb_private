#ifndef TC_INGRESS_CLSACT_FILTER_H
#define TC_INGRESS_CLSACT_FILTER_H


#include <linux/netfilter.h>
#include <linux/types.h>
#include <linux/netfilter_ipv4.h>


// Declare the function that filters packets based on source IP
int tc_ingress_clsact_filter(struct sk_buff *skb, __be32 *value);


// Declare the kfunc wrapper function
__bpf_kfunc int kfunc_tc_ingress_clsact_filter(struct sk_buff *skb, __be32 *value);


#endif /* TC_INGRESS_CLSACT_FILTER_H */