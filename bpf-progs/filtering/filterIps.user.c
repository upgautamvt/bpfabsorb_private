#include <stdio.h>
#include <errno.h>
#include <bpf/bpf.h>
#include <arpa/inet.h>
#include <linux/bpf.h>
#include <string.h>
#include <unistd.h>

// Path to your BPF map (mounted with bpffs)
//this map gets created by bpf kernel program there you have to use __uint(pinning, LIBBPF_PIN_BY_NAME)
#define MAP_PATH "/sys/fs/bpf/xdp/globals/my_map"
#define MAX_ENTRIES 256 // Maximum number of entries in the map
#define MAP_NAME "my_map" // Name of the BPF map

int main() {
    //update 8.8.8.8 value so that it is alternative of "sudo bpftool map update pinned /sys/fs/bpf/xdp/globals/my_map key 00 00 00 00 value 0x08 0x08 0x08 0x08"
    // Open the BPF map
    int map_fd = bpf_obj_get(MAP_PATH);
    if (map_fd < 0) {
        perror("Failed to open BPF map");
        return errno;
    }

    // Prepare the key and value for the map update
    __u32 key = 0; // Using 0 as the key, adjust as necessary
    __be32 value;

    // Convert the IP address 8.8.8.8 to network byte order
    value = htonl(0x08080808); // 8.8.8.8 in hexadecimal is 0x08080808

    // Update the map with the key and value
    if (bpf_map_update_elem(map_fd, &key, &value, BPF_ANY) < 0) {
        perror("Failed to update BPF map");
        close(map_fd);
        return errno;
    }

    printf("Successfully updated the BPF map with IP 8.8.8.8\n");

    // Close the map file descriptor
    close(map_fd);
    return 0;
}

