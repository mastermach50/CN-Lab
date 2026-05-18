#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#define ADDR "127.0.0.1"
#define PORT 8000

typedef struct {
    int type; // 0 Data | 1 ACK
    int seq;
    char data[10];
} Frame;

int main() {

    // Create socket
    int servSock = socket(AF_INET, SOCK_DGRAM, 0);
    if (servSock < 0) {
        perror("Failed to create socket");
        return -1;
    }
    printf("Created socket\n");

    // Set socket timeout
    struct timeval tv = {1,0};
    setsockopt(servSock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

    // Implement Stop and Wait ARQ
    struct sockaddr_in servAddr;
    servAddr.sin_addr.s_addr = inet_addr(ADDR);
    servAddr.sin_port = htons(PORT);
    servAddr.sin_family = AF_INET;
    Frame buffer;
    for (int i = 0; i < 20; i++) {
        pkt_send_begin:
        buffer.type = 0;
        buffer.seq = i;
        strcpy(buffer.data, "hi");
        sendto(
            servSock,
            &buffer,
            sizeof(Frame),
            0,
            (struct sockaddr*) &servAddr,
            sizeof(servAddr)
        );
        printf("> { %d %d %s }\n", buffer.type, buffer.seq, buffer.data);

        int num = recvfrom(
            servSock,
            &buffer,
            sizeof(Frame),
            0,
            NULL,
            NULL
        );
        if (num < 0)
            goto pkt_send_begin;

        if (buffer.seq == i + 1 && buffer.type == 1) {
            printf("< { ACK %d }\n", buffer.seq);
            continue;
        }
    }

    // Close sockets
    close(servSock);
    return 0;
}