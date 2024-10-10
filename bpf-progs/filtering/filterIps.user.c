#include <stdio.h>
#include <errno.h>
#include <bpf/bpf.h>
#include <arpa/inet.h>
#include <linux/bpf.h>
#include <string.h>
#include <unistd.h>

// Path to your BPF map (mounted with bpffs)
#define MAP_PATH "/sys/fs/bpf/xdp/globals/my_map"
#define MAX_ENTRIES 2 // Maximum number of entries in the map
#define MAP_NAME "my_map" // Name of the BPF map

int main(int argc, char *argv[]) {
    // Check for the correct number of arguments
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <IP_ADDRESS>\n", argv[0]);
        return 1;
    }

    // Open the BPF map
    int map_fd = bpf_obj_get(MAP_PATH);
    if (map_fd < 0) {
        perror("Failed to open BPF map");
        return errno;
    }

    // Prepare the key and value for the map update
    __u32 key = 0; // Using 0 as the key, adjust as necessary
    __be32 value;

    // Convert the IP address from string to network byte order
    if (inet_pton(AF_INET, argv[1], &value) != 1) {
        perror("Invalid IP address");
        close(map_fd);
        return errno;
    }

    // Update the map with the key and value
    if (bpf_map_update_elem(map_fd, &key, &value, BPF_ANY) < 0) {
        perror("Failed to update BPF map");
        close(map_fd);
        return errno;
    }

    printf("Successfully updated the BPF map with IP %s\n", argv[1]);

    // Close the map file descriptor
    close(map_fd);
    return 0;
}
