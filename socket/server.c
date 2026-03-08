#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main() {
    // declare connection variables
    int serv_sock, client_sock; // the socket of the server and client
    char buffer[1024]; // the data buffer
    struct sockaddr_in serv_addr, client_addr; // the server and client address
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

    // bind to the socket
    if (bind(serv_sock, (struct sockaddr *) &serv_addr, addr_size) == 0) {
        printf("Binding successful\n");
    } else {
        printf("Binding failed\n");
        return -1;
    }

    // listen for connections
    if (listen(serv_sock, 5) == 0) {
        printf("Listening for connections on 127.0.0.1:9999...\n");
    } else {
        printf("Error listening for connections...\n");
    }

    // accept the connection
    client_sock = accept(serv_sock, (struct sockaddr *) &client_addr, &addr_size);
    printf("Connection accepted\n");

    // receive data
    recv(client_sock, buffer, 1024, 0);
    printf("client> %s", buffer);

    // send data
    strcpy(buffer, "Hi, this is the server!\n");
    send(client_sock, buffer, 1024, 0);

    // close the socket
    close(serv_sock);
    close(client_sock);

    return 0;
}