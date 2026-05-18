#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#define ADDR "127.0.0.1"
#define PORT 8000

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

    // Send/Receive data
    char buffer[100];
    struct sockaddr_in clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);
    ret = recvfrom(servSock, buffer, sizeof(buffer), 0, (struct sockaddr*) &clientAddr, &clientAddrLen);
    if (ret < 0) {
        perror("Failed to receive data");
        return -1;
    }
    printf("Data received from client (%s): %s\n", inet_ntoa(clientAddr.sin_addr), buffer);

    strcpy(buffer, "hello from server");
    ret = sendto(servSock, buffer, sizeof(buffer), 0, (struct sockaddr*) &clientAddr, clientAddrLen);
    if (ret < 0) {
        perror("Failed to send data");
        return -1;
    }
    printf("Data sent to client\n");

    // Close sockets
    close(servSock);
    return 0;
}