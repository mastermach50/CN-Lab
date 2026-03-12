#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main() {
    // declare connection variables
    int serv_sock; // the socket to the server
    char buffer[1024]; // the data buffer
    struct sockaddr_in serv_addr; // the server address
    socklen_t addr_size = sizeof(serv_addr); // the size of the address

    // create the socket
    // AF_INET = regular ipv4 connection
    // SOCK_STREAM = connection oriented
    // 0 = default protocol
    serv_sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (serv_sock < 0) {
        perror("Error creating socket\n");
        return -1;
    } else {
        printf("Socket created\n");
    }

    // set server address
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(9999);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // send data
    strcpy(buffer, "Hi, this is the client!\n");
    sendto(serv_sock, buffer, sizeof(buffer), 0, (struct sockaddr *) &serv_addr, addr_size);

    // recieve data
    recvfrom(serv_sock, buffer, 1024, 0, (struct sockaddr *) &serv_addr, &addr_size);
    printf("server> %s\n", buffer);

    // close the socket
    close(serv_sock);

    return 0;
}