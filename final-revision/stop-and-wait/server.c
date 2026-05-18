#include <stdio.h>
#include <stdlib.h>
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

    // Bind socket
    struct sockaddr_in servAddr;
    servAddr.sin_addr.s_addr = inet_addr(ADDR);
    servAddr.sin_port = htons(PORT);
    servAddr.sin_family = AF_INET;
    int ret = bind(servSock, (struct sockaddr*)&servAddr, sizeof(servAddr));
    if (ret < 0) {
        perror("Failed to bind socket");
        return -1;
    }
    printf("Socket bound to %s:%d\n", ADDR, PORT);

    // Send ACKs for every frame received (almost)
    Frame buffer[sizeof(Frame)];
    struct sockaddr clientAddr;
    socklen_t clientAddrLen;
    int seq = 0;
    while (1) {
        int num = recvfrom(
            servSock,
            buffer,
            sizeof(Frame),
            0,
            &clientAddr,
            &clientAddrLen
        );

        if (num > 0) {
            printf("< { %d %d %s }\n", buffer->type, buffer->seq, buffer->data);
            // if the frame is in the correct sequence and we are lucky then send ACK
            if (buffer->seq == seq & (rand() % 100) < 80) {
                buffer->type = 1;
                buffer->seq = seq+1;
                strcpy(buffer->data, "");
                int num = sendto(
                    servSock,
                    buffer,
                    sizeof(Frame),
                    0,
                    &clientAddr,
                    clientAddrLen
                );
                printf("> { ACK %d }\n", buffer->seq);

                seq++;
            }
        }

    }

    // Close sockets
    close(servSock);
    return 0;
}