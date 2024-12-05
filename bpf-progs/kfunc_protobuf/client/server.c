// For server
// 1. open a socket
// 2. wait for someone to connect
// 3. decode the protobuf
// 4. print out the name
// 5. goto 2


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
    uint8_t message[256];
    struct dropme_foo_t *foo_p;

    int total_requests = 0;
    if (argc != 3) {
	    printf("Failed need args\n");
	    return -1;
    }

    char * ip = argv[1];
    char * port_string = argv[2]; 
    int port = atoi(port_string);
    printf("%s and port %d\n", ip, port);
    // Get the socket
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("Socket creation failed");
        return -1;
    }
    memset(&server_addr, 0, sizeof(server_addr));  // Zero out structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(ip);
    //if (inet_pton(AF_INET, ip, &server_addr.sin_addr) <= 0) {
    //    perror("Invalid address/ Address not supported");
    //    close(sock);
    //    return -1;
    //}
    // bind to the port
    if (bind(sock, (struct sockaddr *)&server_addr, sizeof server_addr) == -1) {
	    printf("failed to bind\n");
	    return -1;
    }

    //if (listen(sock, 0) == -1) {
    //        printf("listen failed\n");
    //        return -1;
    //}

    struct sockaddr_storage their_addr;
    int new_fd;
    socklen_t sin_size;


    while (1) {
	    sin_size = sizeof(their_addr);
	    //new_fd = accept(sock, (struct sockaddr *)&their_addr, &sin_size);
	    //if (new_fd == -1) {
 		//	    printf("error accept\n");
		 //   continue;
	    //}

	    int bytes = recvfrom(sock, message, 255, 0, (struct sockaddr *)&their_addr, &sin_size); 
	    if (bytes == -1) {
		    printf("rec err\n");
		    continue;
	    }

    	    foo_p = dropme_foo_new(&workspace[0], sizeof(workspace));
	   
	    if (foo_p == NULL) {
		    printf("err\n");
		    continue;
	    }
	    int s = *message;
   	    size = dropme_foo_decode(foo_p, (message + 4), s);
	    printf("Got request for name %s\n", foo_p->name_p);
	    total_requests++;
	    close(new_fd);
    		printf("Total requests was %d\n", total_requests);
    }
    close(sock);
    return 0;
}
