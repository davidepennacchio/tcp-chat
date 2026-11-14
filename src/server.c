#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>


int initServerFileDescriptor(int port, struct sockaddr_in *address);
int acceptClient(int server_fd, struct sockaddr_in address, socklen_t addrlen);

int main(void)
{
    int server_fd;
    struct sockaddr_in address;
    if ((server_fd = initServerFileDescriptor(8080, &address)) < 0)
    {
   		perror("error while creating server socket");
     	exit(EXIT_FAILURE);
    }
    
    socklen_t addrlen = sizeof(address);
    int client_fd = acceptClient(server_fd, address, addrlen);
    

    struct pollfd pfd;
    //char buf[128];
    //ssize_t n;

    pfd.fd = STDIN_FILENO;
    pfd.events = POLLIN;
    pfd.revents = 0;
    
    while (1)
    {
    	char *client_message = NULL;
     	char *server_msg = NULL;
      	size_t server_msg_len = 0;
       
    	if (read(client_fd, client_message, sizeof(client_message) - 1) <= 0)
        {
        	perror("error while reading reiceveing message");
         	exit(EXIT_FAILURE);
        }

     	client_message[strlen(client_message)] = '\0';
        printf("%s\n", client_message);
          
      	if (getline(&server_msg, &server_msg_len, stdin) == -1)
        {
      	   	perror("error while reading input");
           	free(server_msg);
            exit(EXIT_FAILURE);
        }
      	if (server_msg_len != 0)
       	{
        	send(client_fd, server_msg, strlen(server_msg), 0);
        }
    }
    close(client_fd);
    close(server_fd);
	return 0;
}

int initServerFileDescriptor(int port, struct sockaddr_in *address)
{
	int opt = 1;

	int server_fd;

	if ((server_fd = socket(PF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("error while creating socket file descriptor");
        return -1;
	}
	
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
	{
		perror("error while setting socket options");
        return -1;
	}
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt)))
	{
		perror("error while setting socket options");
        return -1;
	}

	address->sin_family = AF_INET;
    address->sin_addr.s_addr = INADDR_ANY;
    address->sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr*)&(*address), sizeof(*address)) < 0)
    {
   		perror("error while binding server socket to address");
        return -1;
    }

    if (listen(server_fd, 3) < 0) 
    {
        perror("error while setting server socket to listen mode");
        return -1;
    }
    return server_fd;
}

int acceptClient(int server_fd, struct sockaddr_in address, socklen_t addrlen)
{
	int client_fd;
	if ((client_fd = accept(server_fd, (struct sockaddr*)&address, &addrlen)) < 0)
    {
        perror("error while creating client socket");
        return -1;
    }
	return client_fd;
}