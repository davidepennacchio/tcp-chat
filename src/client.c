#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
	// Declaring the needed sokets
    int client_fd;

    // Declaring address to which the clent socket will connect
    struct sockaddr_in serv_addr;
    
    char* hello = "Hello from client";
    char buffer[1024] = { 0 };

    // Assigning client_fd socket file descriptor
    // Checking if file descriptor is less than 0 (error while creating socket file descriptor)
    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    {
    	perror("error while creating socket file descriptor");
        exit(EXIT_FAILURE);
    }

    // Assigning address values
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080);

    // Converting a presentation format address to network format with inet_pton()
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) 
    {
   		perror("error while converting presentation format to network format");
        exit(EXIT_FAILURE);
    }

    // Connecting socket client to address
    // Checking if connect return is less than 0 (error while connecting client socket to address)
    if (connect(client_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) 
    {
    	perror("error while connecting client socket to address");
        exit(EXIT_FAILURE);
    }
  
    send(client_fd, hello, strlen(hello), 0);
    printf("Hello message sent\n");
    read(client_fd, buffer, 1024 - 1); 
    printf("%s\n", buffer);

    // Closing sockets connections and file descriptors
    close(client_fd);
    return 0;
}