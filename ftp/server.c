#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
    int serv_sock, cl_sock;
    struct sockaddr_in serv_addr, cl_addr;
    socklen_t addr_len=sizeof(cl_addr);
    
    serv_addr.sin_family=AF_INET;
    serv_addr.sin_port=htons(8000);
    serv_addr.sin_addr.s_addr=inet_addr("127.0.0.1");

    serv_sock=socket(AF_INET,SOCK_STREAM,0);

    if (bind(serv_sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr))==0){
        printf("Connected\n");
    }
    else{
        perror("Bind Fail");
    }

    if (listen(serv_sock,5)==0){
        printf("Listening\n");
    }
    else{
        perror("Listen Fail\n");
    }

   char buffer[1024];

    cl_sock = accept(serv_sock,(struct sockaddr *) &cl_addr, &addr_len);

    recv(cl_sock,buffer,sizeof(buffer),0);

    FILE *fp = fopen(buffer,"r");
    while(1){
        int n =fread(buffer, sizeof(char), sizeof(buffer),fp);
        send(cl_sock,buffer, n,0);

        if (n < sizeof(buffer)) {
            break;
        }
    }

    fclose(fp);
    close(serv_sock);
    close(cl_sock);
}