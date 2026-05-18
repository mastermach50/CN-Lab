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
    int servSock = socket(AF_INET, SOCK_STREAM, 0);
    if (servSock < 0) {
        perror("Failed to create socket");
        return -1;
    }
    printf("Created socket\n");

    // Connect using socket
    struct sockaddr_in servAddr;
    servAddr.sin_addr.s_addr = inet_addr(ADDR);
    servAddr.sin_port = htons(PORT);
    servAddr.sin_family = AF_INET;
    int ret = connect(servSock, (struct sockaddr*) &servAddr, sizeof(servAddr));
    if (ret < 0) {
        perror("Failed to connect to server");
        return -1;
    }
    printf("Connected to server\n");

    // Send/Receive data
    char buffer[100];
    ret = recv(servSock, buffer, sizeof(buffer), 0);
    if (ret < 0) {
        perror("Failed to receive data");
        return -1;
    }
    printf("Data received from server: %s\n", buffer);
    strcpy(buffer, "hi this is the client");
    ret = send(servSock, buffer, sizeof(buffer), 0);
    if (ret < 0) {
        perror("Failed to send data");
        return -1;
    }
    printf("Data sent to server\n");

    // Close sockets
    close(servSock);
    return 0;
}