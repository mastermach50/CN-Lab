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

int iseven(int n) {
    if (n % 2 == 0)
        return 1;
    else
        return 0;
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

    if (connect(
        servSock,
        (struct sockaddr*) &servAddr,
        sizeof(servAddr)) != -1
    ) {
        printf("Connected to server\n");
    } else {
        perror("Failed to connect to server\n");
        return -1;
    }


    int buf = 23;
    // Send number and receive result
    if (send(servSock, &buf, sizeof(buf), 0) != -1) {
        printf("Sent number %d\n", buf);
        if (recv(servSock, &buf, sizeof(buf), 0) != -1) {
            printf("Recieved value %d\n", buf);
        } else {
            perror("Failed to receive data\n");
            return -1;
        }
    } else {
        perror("Failed to send data\n");
        return -1;
    }

    // Receive number to test and send result
    if (recv(servSock, &buf, sizeof(buf), 0)) {
        printf("Received number %d\n", buf);
        buf = iseven(buf);
        if (send(servSock, &buf, sizeof(buf), 0) != -1 ) {
            printf("Sent result %d\n", buf);
        } else {
            perror("Failed to send result\n");
            return -1;
        }
    } else {
        perror("Failed to receive number\n");
        return -1;
    }

    close(servSock);

    return 0;
}