#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main() {
    int serv_sock, client_sock;
    char buffer[1024];
    struct sockaddr_in serv_addr, client_addr;
    socklen_t addr_size = sizeof(serv_addr);

    // 1. socket()
    serv_sock = socket(AF_INET, SOCK_STREAM, 0);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(9999);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // 2. bind()
    bind(serv_sock, (struct sockaddr *) &serv_addr, addr_size);

    // 3. listen()
    listen(serv_sock, 5);

    // 4. accept()
    client_sock = accept(serv_sock, (struct sockaddr *) &client_addr, &addr_size);

    // 5. send() / recv()
    recv(client_sock, buffer, 1024, 0);
    printf("client> %s", buffer);
    strcpy(buffer, "Hi, this is the server!\n");
    send(client_sock, buffer, 1024, 0);

    // 6. close()
    close(serv_sock);
    close(client_sock);

    return 0;
}