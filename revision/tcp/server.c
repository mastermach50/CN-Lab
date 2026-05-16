// Write a tcp/udp server and client such that client should pass on
// a number and the server should tell if the number is prime or not.
// After sending it's verdict to the client, the server should also
// pass a number and client should tell the server if that number
// is even or not

#include <arpa/inet.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

int isprime(int n) {
    if (n < 2)
        return 0;
    for (int i = 2; i < n/2; i++) {
        if (n % i == 0)
            return 0;
    }

    return 1;
}

int main() {

    // Create socket
    int servSock = socket(AF_INET, SOCK_STREAM, 0);
    if (servSock != -1) {
        printf("Created socket\n");
    } else {
        perror("Failed to create socket\n");
        return -1;
    }

    // Set address
    struct sockaddr_in servAddr;
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = inet_addr("0.0.0.0");
    servAddr.sin_port = htons(8000);

    // Bind to address
    if (bind(
        servSock,
        (struct sockaddr*) &servAddr,
        sizeof(servAddr)) != -1
    ) {
        printf("Socket bound\n");
    } else {
        perror("Failed to bind\n");
        return -1;
    }

    // Listen for connections
    if (listen(servSock, 5) != -1) {
        printf("Listening for connections\n");
    } else {
        perror("Failed to start listening\n");
        return -1;
    }

    // Loop for accepting connections
    while (1) {
        struct sockaddr_in clientAddr;
        socklen_t clientAddrSize;
        int clientSock = accept(servSock, (struct sockaddr*) &clientAddr, &clientAddrSize);

        if (clientSock != -1) {
            printf("Connection Accepted from %d\n", clientAddr.sin_addr.s_addr);
        } else {
            perror("Failed to accept connection\n");
            continue;
        }

        int buf;
        // Recieve number to test and send result
        if (recv(clientSock, &buf, sizeof(buf), 0) != -1) {
            printf("Received number %d\n", buf);
            buf = isprime(buf);
            if (send(clientSock, &buf, sizeof(buf), 0) != -1) {
                printf("Sent value %d\n", buf);
            } else {
                perror("Failed to send data\n");
                continue;
            }
        } else {
            perror("Failed to recieve data\n");
            continue;
        }

        // Send number to test and recieve result
        buf = 10;
        if (send(clientSock, &buf, sizeof(buf), 0)) {
            printf("Sent number %d\n", buf);
            if (recv(clientSock, &buf, sizeof(buf), 0) != -1 ) {
                printf("Received result %d\n", buf);
            } else {
                perror("Failed to recieve result\n");
                continue;
            }
        } else {
            perror("Failed to send number\n");
            continue;
        }

        close(clientSock);
        break;
    }

    close(servSock);

    return 0;
}