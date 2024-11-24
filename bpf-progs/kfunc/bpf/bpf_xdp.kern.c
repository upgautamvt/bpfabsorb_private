#include <linux/bpf.h>
#include <linux/if_ether.h>
#include <linux/if_vlan.h>
#include <linux/ip.h>
#include <linux/udp.h>
#include <bpf/bpf_helpers.h>

extern void *my_kfunc_memcpy(void *dst, __u32 dst__sz, void *src, __u32 src__sz) __ksym;
#define MEMCPY(...) my_kfunc_memcpy(__VA_ARGS__)

struct item {
    char data[1000];
};

struct {
    __uint(type, BPF_MAP_TYPE_ARRAY);
    __type(key,  __u32);
    __type(value, struct item);
    __uint(max_entries, 10);
} a_map SEC(".maps");

SEC("xdp")
int prog(struct xdp_md *xdp)
{
    int i = 0, ii = 1;
    struct item *d = bpf_map_lookup_elem(&a_map, &i);
    if (d == NULL) return XDP_PASS;
    struct item *it = bpf_map_lookup_elem(&a_map, &ii);
    if (it == NULL) return XDP_PASS;
    MEMCPY(it->data, 1000, d->data, 1000);
    return XDP_PASS;
}

char _license[] SEC("license") = "GPL";

/*
cat /proc/kallsyms | grep my_kfunc_memcpy
ffffffffa0205008 r __BTF_ID__func__my_kfunc_memcpy__26427       [xdp_kfunc_module]
ffffffffa02050f0 r my_kfunc_memcpy_kfunc_set    [xdp_kfunc_module]
ffffffffa0201050 t my_kfunc_memcpy      [xdp_kfunc_module]

/proc/kallsyms is a virtual file in the Linux kernel that contains a list of all
kernel symbols that are currently loaded in memory. These symbols include functions,
variables, and other objects, along with their corresponding memory addresses.

vmlinux is the uncompressed and complete kernel image that includes the compiled machine
code (text section), kernel symbols, and other debug information (such as BTF metadata).
BTF includes type information (like struct definitions and function signatures).

Ok, since we see my_kfunc_memcpy kfunc in /proc/kallsyms, but we don't see in the vmlinux.
Seeing in /proc/kallsysm means the kernel module has successfully registered the function
as a symbol that can be used by other parts of the kernel, but not from BPF system.
For BPF programs to access a kfunc, it must also be registered in the BTF metadata.

BPF programs don’t directly interact with kernel symbols via /proc/kallsyms the way other parts of the kernel might.
Instead, BPF requires BTF (BPF Type Format) metadata to know the types and signatures of kernel functions, including kfuncs.

If the kfunc does not show up in vmlinux (or the BTF dump) but is in /proc/kallsyms,
it likely means that the function has not been registered for BPF or the BTF
metadata has not been correctly generated for it.


bpftool btf dump file /sys/kernel/btf/vmlinux | grep my_kfunc_memcpy
It doesn't show kfunc even after doing "cp /sys/kernel/btf/vmlinux /usr/lib/modules/$(uname -r)/build/" and installing dwarves
It means, I think, I need to use at least linux version 6.11.
 */