#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define PORT 9999
#define BUF_SIZE 4096

int send_reply(int sock, char *msg) {
    send(sock, msg, strlen(msg), 0);
    printf("server> %s", msg);
    return 0;
}

int main() {
    int server_fd, client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    char buffer[BUF_SIZE];

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("Error creating socket");
        return -1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        return -1;
    }

    if (listen(server_fd, 1) < 0) {
        perror("Listen failed");
        return -1;
    }

    printf("SMTP mock server listening on port %d...\n", PORT);

    client_sock = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
    if (client_sock < 0) {
        perror("Accept failed");
        return -1;
    }
    printf("Client connected.\n\n");

    // S: 220 greeting — sent immediately on connect, before any client message
    send_reply(client_sock, "220 smtp.example.com ESMTP Postfix\r\n");

    int in_data = 0; // tracks whether we're in DATA body mode

    while (1) {
        memset(buffer, 0, BUF_SIZE);
        int bytes = recv(client_sock, buffer, BUF_SIZE - 1, 0);
        if (bytes <= 0) {
            printf("Client disconnected.\n");
            break;
        }

        printf("client> %s", buffer);

        if (in_data) {
            // The DATA body ends when the client sends a lone "."
            // The client in this code sends the body + "." in one send(),
            // so we check if the buffer ends with \r\n.\0 or just "."
            if (strstr(buffer, "\r\n.") != NULL || strcmp(buffer, ".") == 0) {
                in_data = 0;
                send_reply(client_sock, "250 OK: queued as 12345\r\n");
            }
            // If it's a multi-send DATA body, just keep reading until "."
        } else if (strncmp(buffer, "HELO", 4) == 0) {
            send_reply(client_sock, "250 Hello relay.example.org, I am glad to meet you\r\n");
        } else if (strncmp(buffer, "EHLO", 4) == 0) {
            send_reply(client_sock, "250 Hello relay.example.org, I am glad to meet you\r\n");
        } else if (strncmp(buffer, "MAIL FROM:", 10) == 0) {
            send_reply(client_sock, "250 OK\r\n");
        } else if (strncmp(buffer, "RCPT TO:", 8) == 0) {
            send_reply(client_sock, "250 OK\r\n");
        } else if (strncmp(buffer, "DATA", 4) == 0) {
            in_data = 1;
            send_reply(client_sock, "354 End data with <CR><LF>.<CR><LF>\r\n");
        } else if (strncmp(buffer, "QUIT", 4) == 0) {
            send_reply(client_sock, "221 Bye\r\n");
            break;
        } else {
            send_reply(client_sock, "500 Command unrecognized\r\n");
        }
    }

    close(client_sock);
    close(server_fd);
    return 0;
}