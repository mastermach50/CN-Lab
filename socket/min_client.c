#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main() {
    int serv_sock;
    char buffer[1024];
    struct sockaddr_in serv_addr;
    socklen_t addr_size = sizeof(serv_addr);

    // 1. socket()
    serv_sock = socket(AF_INET, SOCK_STREAM, 0);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(9999);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // 2. connect()
    connect(serv_sock, (struct sockaddr *) &serv_addr, addr_size);

    // 3. send() / recv()
    strcpy(buffer, "Hi, this is the client!\n");
    send(serv_sock, buffer, strlen(buffer), 0);
    recv(serv_sock, buffer, 1024, 0);
    printf("server> %s\n", buffer);

    // 4. close()
    close(serv_sock);

    return 0;
}