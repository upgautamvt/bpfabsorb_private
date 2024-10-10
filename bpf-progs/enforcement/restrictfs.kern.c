//
// Created by upgautamvt on 8/30/24.
//

/**
* @link: https://kinvolk.io/blog/2021/02/extending-systemd-security-features-with-ebpf
* @link: https://github.com/systemd/systemd/blob/e5191faf44bfcb709b1452890ef3fec9a7428055/src/core/bpf/restrict_fs/restrict-fs.bpf.c
*/

/* SPDX-License-Identifier: LGPL-2.1-or-later */

/* The SPDX header above is actually correct in claiming this was
 * LGPL-2.1-or-later, because it is. Since the kernel doesn't consider that
 * compatible with GPL we will claim this to be GPL however, which should be
 * fine given that LGPL-2.1-or-later downgrades to GPL if needed.
 */

 /*
 This program decides if a process can access different filesystem.
 Each filesystem has their own magic number as their unique id.
 Cgroup groups processes in the system.
 So, this bpf program checks if a Cgroup (processes that are part of the Cgroup) can access or not some filesystem.
  */

#include <linux/types.h>
#include <linux/bpf.h>
#include <bpf/bpf_helpers.h>
#include <bpf/bpf_tracing.h>
#include <bpf/bpf_core_read.h>
#include <errno.h>
#include <stddef.h>
#include <stdint.h>

struct super_block {
        unsigned long int s_magic;
} __attribute__((preserve_access_index));

struct inode {
        struct super_block *i_sb;
} __attribute__((preserve_access_index));

struct file {
        struct inode *f_inode;
} __attribute__((preserve_access_index));

struct inner_map {
        __uint(type, BPF_MAP_TYPE_ARRAY);
        __uint(max_entries, 1);
        __type(key, int);
        __type(value, __u64);
} inner_map SEC(".maps");

struct cgroup_hash {
	__uint(type, BPF_MAP_TYPE_HASH_OF_MAPS);
	__uint(max_entries, 2048);
	__uint(key_size, sizeof(int));
	__array(values, struct inner_map);
} cgroup_hash SEC(".maps") = {
	.values = { [0] = &inner_map },
};

//lsm hook to the file open event
SEC("lsm/file_open")
int BPF_PROG(restrict_filesystems, struct file *file, int ret)
{
        unsigned long raw_magic_number;
        uint64_t cgroup_id;
        uint32_t *value, *magic_map, magic_number, zero = 0, *is_allow;
        /* ret is the return value from the previous BPF program or 0 if it's
         * the first hook */
        bpf_printk("LSM program is running\n");

        if (ret != 0)
                return ret;

        BPF_CORE_READ_INTO(&magic_number, file, f_inode, i_sb, s_magic);

        cgroup_id = bpf_get_current_cgroup_id();

        magic_map = bpf_map_lookup_elem(&cgroup_hash, &cgroup_id);
        if (!magic_map)
                return 0;

        if ((is_allow = bpf_map_lookup_elem(magic_map, &zero)) == NULL) {
                /* Malformed map, it doesn't include whether it's an allow list
                 * or a deny list. Allow. */
                return 0;
        }

        if (*is_allow) {
                /* Allow-list: Allow access only if magic_number present in inner map */
                if (bpf_map_lookup_elem(magic_map, &magic_number) == NULL)
                        return -EPERM;
        } else {
                /* Deny-list: Allow access only if magic_number is not present in inner map */
                if (bpf_map_lookup_elem(magic_map, &magic_number) != NULL)
                        return -EPERM;
        }

        return 0;
}

static const char _license[] SEC("license") = "GPL";
