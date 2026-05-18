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

    // Listen on socket
    ret = listen(servSock, 5);
    if (ret < 0) {
        perror("Failed to listen on socket");
        return -1;
    }
    printf("Listening for connections\n");

    // Accept connections
    int clientSock;
    struct sockaddr_in clientAddr;
    socklen_t clientAddrSize = sizeof(clientAddr);
    clientSock = accept(servSock, (struct sockaddr*) &clientAddr, &clientAddrSize);
    if (clientSock < 0) {
        perror("Failed to accept connection");
        return -1;
    }
    printf("Accepted connection from %s\n", inet_ntoa(clientAddr.sin_addr));

    // Send/Receive data
    char buffer[100];
    strcpy(buffer, "hello from server");
    ret = send(clientSock, buffer, sizeof(buffer), 0);
    if (ret < 0) {
        perror("Failed to send data");
        return -1;
    }
    printf("Data sent to client\n");
    ret = recv(clientSock, buffer, sizeof(buffer), 0);
    if (ret < 0) {
        perror("Failed to receive data");
        return -1;
    }
    printf("Data received from client: %s\n", buffer);

    // Close sockets
    close(clientSock);
    close(servSock);
    return 0;
}