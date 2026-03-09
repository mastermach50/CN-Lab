#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

int serv_sock;

int send_msg(char *msg) {
    if (msg != NULL) {
        send(serv_sock, msg, strlen(msg), 0);
        printf("client> %s", msg);
    }

    char buffer[1024];
    recv(serv_sock, buffer, 1024, 0);
    printf("server> %s\n", buffer);

    return 0;
}

int main() {
    struct sockaddr_in serv_addr;
    socklen_t addr_size = sizeof(serv_addr);

    serv_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (serv_sock < 0) {
        perror("Error creating socket\n");
        return -1;
    } else {
        printf("Socket created\n");
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(9999);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    printf("Connecting to 127.0.0.1:9999...\n");
    if (connect(serv_sock, (struct sockaddr *)&serv_addr, addr_size) == 0) {
        printf("Connection successful\n");
    } else {
        perror("Connection failed\n");
        return -1;
    }

    // S: 220 smtp.example.com ESMTP Postfix
    send_msg(NULL);

    // C: HELO relay.example.org
    // S: 250 Hello relay.example.org, I am glad to meet you
    send_msg("HELO relay.example.org\r\n");

    // C: MAIL FROM:<bob@example.org>
    // S: 250 OK
    send_msg("MAIL FROM:<bob@example.org>\r\n");

    // C: RCPT TO:<alice@example.com>
    // S: 250 OK
    send_msg("RCPT TO:<alice@example.com>\r\n");

    // C: RCPT TO:<theboss@example.com>
    // S: 250 OK
    send_msg("RCPT TO:<theboss@example.com>\r\n");

    // C: DATA
    // S: 354 End data with <CR><LF>.<CR><LF>
    send_msg("DATA\r\n");

    // C: <email>
    // S: 250 OK: queued as 12345
    char *email = 
    "From: Bob Example <bob@example.org>\r\n"
    "To: Alice Example <alice@example.com>\r\n"
    "Cc: theboss@example.com\r\n"
    "Date: Tue, 15 Jan 2008 16:02:43 -0500\r\n"
    "Subject: Test message\r\n"
    "\r\n"
    "Hello Alice.\r\n"
    "\r\n"
    ".";
    send_msg(email);

    // C: QUIT
    // S: 221 Bye
    send_msg("QUIT\r\n");

    close(serv_sock);

    return 0;
}