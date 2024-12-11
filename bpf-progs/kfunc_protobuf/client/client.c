#include <stdio.h>
#include "dropme.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define SERVER_IP "192.168.1.2"
#define SERVER_PORT 8080
int send_payload(int sock, const uint8_t *payload, int size, struct sockaddr_in *server_addr) {
    if (sendto(sock, payload, size, 0, (struct sockaddr *)server_addr, sizeof(*server_addr)) != size) {
        perror("Failed to send the entire payload");
        return -1;
    }
    printf("Successfully sent %d bytes to the server.\n", size);
    return 0;
}
int main(int argc, char *argv[]) {
    int sock;
    struct sockaddr_in server_addr;
    int size;
    uint8_t workspace[64];
    uint8_t encoded[16];
    uint8_t modified_encoded[20];
    struct dropme_foo_t *foo_p;

    if (argc != 3) {
	    printf("Failed need args\n");
	    return -1;
    }
    char * ip = argv[1];
    char * port_string = argv[2]; 
    int port = atoi(port_string);
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("Socket creation failed");
        return -1;
    }
    // Set up server address
    memset(&server_addr, 0, sizeof(server_addr));  // Zero out structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    if (inet_pton(AF_INET, ip, &server_addr.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
        close(sock);
        return -1;
    }
    for (int i = 0; i < 20; i++) {
    // Encode (using your existing encoding logic)
    foo_p = dropme_foo_new(&workspace[0], sizeof(workspace));
    if (foo_p == NULL) {
        close(sock);
        return 1;
    }
    foo_p->drop = 3;
    foo_p->name_p = "GOOD";
    size = dropme_foo_encode(foo_p, &encoded[0], sizeof(encoded));
    if (size < 0) {
        close(sock);
        return 2;
    }
    printf("Successfully encoded Foo into %d bytes.\n", size);
    //4 bytes size = this is length of encoded message
    //remaining is encoded message
    memcpy(modified_encoded, encoded, 4);               // Copy first 4 bytes
    memcpy(modified_encoded + 4, encoded, size);        // Copy the rest of encoded to modified_encoded
    if (send_payload(sock, modified_encoded, size+4, &server_addr) != 0) {
        close(sock);
        return 3;
    }
    foo_p = dropme_foo_new(&workspace[0], sizeof(workspace));
    if (foo_p == NULL) {
        close(sock);
        return 1;
    }
    foo_p->drop = 1;
    foo_p->name_p = "BAD";
    size = dropme_foo_encode(foo_p, &encoded[0], sizeof(encoded));
    if (size < 0) {
        close(sock);
        return 2;
    }
    //printf("Successfully encoded Foo into %d bytes.\n", size);
    //4 bytes size = this is length of encoded message
    //remaining is encoded message
    memcpy(modified_encoded, encoded, 4);               // Copy first 4 bytes
    memcpy(modified_encoded + 4, encoded, size);        // Copy the rest of encoded to modified_encoded
    if (send_payload(sock, modified_encoded, size+4, &server_addr) != 0) {
        close(sock);
        return 3;
    }
    }
    close(sock);
    return 0;
}
