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
    serv_sock = socket(AF_INET, SOCK_STREAM, 0);

    // set server address
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(9999);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // connect to the server
    printf("Connecting to 127.0.0.1:9999...\n");
    if (connect(serv_sock, (struct sockaddr *) &serv_addr, addr_size) == 0) {
        printf("Connection successful\n");
    } else {
        printf("Connection failed\n");
        return -1;
    }

    // send data
    strcpy(buffer, "Hi, this is the client!\n");
    send(serv_sock, buffer, 1024, 0);

    // recieve data
    recv(serv_sock, buffer, 1024, 0);
    printf("server> %s\n", buffer);

    // close the socket
    close(serv_sock);

    return 0;
}