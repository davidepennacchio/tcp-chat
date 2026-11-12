#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
    int status, client_fd;
    struct sockaddr_in serv_addr;
    char* hello = "Hello from client";
    char buffer[1024] = { 0 };
    
    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    {
    	perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) 
    {
   		perror("inet_pton failed");
        exit(EXIT_FAILURE);
    }

    if ((status = connect(client_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr))) < 0) 
    {
    	perror("connection failed");
        exit(EXIT_FAILURE);
    }
  
    send(client_fd, hello, strlen(hello), 0);
    printf("Hello message sent\n");
    read(client_fd, buffer, 1024 - 1); 
    printf("%s\n", buffer);

    close(client_fd);
    return 0;
}