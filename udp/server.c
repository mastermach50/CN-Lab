#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main() {
    // declare connection variables
    int serv_sock; // the socket of the server
    char buffer[1024]; // the data buffer
    struct sockaddr_in serv_addr, client_addr; // the server and client address
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

    // bind to the socket
    if (bind(serv_sock, (struct sockaddr *) &serv_addr, addr_size) == 0) {
        printf("Binding successful\n");
    } else {
        perror("Binding failed\n");
        return -1;
    }

    // receive data
    recvfrom(serv_sock, buffer, 1024, 0, (struct sockaddr *) &client_addr, &addr_size);
    printf("client> %s", buffer);

    // send data
    strcpy(buffer, "Hi, this is the server!\n");
    sendto(serv_sock, buffer, 1024, 0, (struct sockaddr *) &client_addr, addr_size);

    // close the socket
    close(serv_sock);

    return 0;
}