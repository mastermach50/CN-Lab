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

    // Send/Receive data
    struct sockaddr_in servAddr;
    servAddr.sin_addr.s_addr = inet_addr(ADDR);
    servAddr.sin_port = htons(PORT);
    servAddr.sin_family = AF_INET;
    char buffer[100];
    strcpy(buffer, "hi this is the client");
    int ret = sendto(servSock, buffer, sizeof(buffer), 0, (struct sockaddr*) &servAddr, sizeof(servAddr));
    if (ret < 0) {
        perror("Failed to send data");
        return -1;
    }
    printf("Data sent to server\n");

    ret = recvfrom(servSock, buffer, sizeof(buffer), 0, NULL, NULL);
    if (ret < 0) {
        perror("Failed to receive data");
        return -1;
    }
    printf("Data received from server: %s\n", buffer);

    // Close sockets
    close(servSock);
    return 0;
}