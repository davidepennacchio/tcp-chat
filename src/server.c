#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


int main(void)
{
	int opt = 1;
	struct sockaddr_in address;
	socklen_t addrlen = sizeof(address);
	
	int server_fd, new_socket;

	char buffer[1024] = { 0 };
    char *hello = "Hello from server";

	if ((server_fd = socket(PF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("socket creation failed");
        exit(EXIT_FAILURE);
	}

	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
	{
		perror("setsockopt failed");
        exit(EXIT_FAILURE);
	}

	address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0)
    {
   		perror("bind failed");
        exit(EXIT_FAILURE);
    }
    
    if (listen(server_fd, 3) < 0) 
    {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    if ((new_socket = accept(server_fd, (struct sockaddr*)&address, &addrlen)) < 0)
    {
        perror("accept failed");
        exit(EXIT_FAILURE);
    }

    read(new_socket, buffer, 1024 - 1); 
    printf("%s\n", buffer);
    send(new_socket, hello, strlen(hello), 0);
    printf("Hello message sent\n");

    close(new_socket);
    close(server_fd);
    
	return 0;
}