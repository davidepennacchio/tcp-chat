#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>


int main(void)
{
	// Socket option value declaring and assigning
	int opt = 1;

	// Declaring address to which the server socket will bind to and defining its length
	struct sockaddr_in address;
	socklen_t addrlen = sizeof(address);

	// Declaring the needed sokets
	int server_fd, client_fd;

	char buffer[1024] = { 0 };
    char *hello = "Hello from server";

    // Assigning server_fd socket file descriptor
    // Checking if file descriptor is less than 0 (error while creating socket file descriptor)
	if ((server_fd = socket(PF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("error while creating socket file descriptor");
        exit(EXIT_FAILURE);
	}
	
	// Defining server_fd socket options
    // Checking if option setting is different than 0 (error while setting socket options)
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
	{
		perror("error while setting socket options");
        exit(EXIT_FAILURE);
	}
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt)))
	{
		perror("error while setting socket options");
        exit(EXIT_FAILURE);
	}

	// Assigning address values
	address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    // Binding socket server to address
    // Checking if bind return is less than 0 (error while binding server socket to address)
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0)
    {
   		perror("error while binding server socket to address");
        exit(EXIT_FAILURE);
    }

    // Setting server socket into listening mode
    // Checking if listen return is less than 0 (error while setting server socket to listen mode)
    if (listen(server_fd, 3) < 0) 
    {
        perror("error while setting server socket to listen mode");
        exit(EXIT_FAILURE);
    }

    // Assigning new socket file descriptor while accepting client connection
    // Checking if new fd is less than 0 (error while creating client socket)
    if ((client_fd = accept(server_fd, (struct sockaddr*)&address, &addrlen)) < 0)
    {
        perror("error while creating client socket");
        exit(EXIT_FAILURE);
    }

    read(client_fd, buffer, 1024 - 1); 
    printf("%s\n", buffer);
    send(client_fd, hello, strlen(hello), 0);
    printf("Hello message sent\n");

    // Closing sockets connections and file descriptors
    close(client_fd);
    close(server_fd);
    
	return 0;
}