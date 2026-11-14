#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
    int client_fd;
    struct sockaddr_in serv_addr;
    
    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    {
    	perror("error while creating socket file descriptor");
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) 
    {
   		perror("error while converting presentation format to network format");
        exit(EXIT_FAILURE);
    }
    
    while (connect(client_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)))
    {
    	char *server_message = NULL;
     	char *client_msg = NULL;
      	size_t client_message_len = 0;
       
    	if (read(client_fd, server_message, sizeof(server_message) - 1) <= 0)
       	{
       		perror("error while reading reiceveing message");
        	exit(EXIT_FAILURE);
        }
      
     	server_message[strlen(server_message) - 1] = '\0';
        printf("%s\n", server_message);
        
      	if (getline(&client_msg, &client_message_len, stdin) == -1)
        {
       	   perror("error while reading input");
           free(client_msg);
           exit(EXIT_FAILURE);
        }
       
      	if (client_message_len != 0)
       	{
        	send(client_fd, client_msg, strlen(client_msg), 0);
        }
    }
    close(client_fd);
    return 0;
}