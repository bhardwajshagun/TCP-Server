// Our standard libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Some types and unix operations
#include <sys/types.h>
#include <unistd.h>

// A sockets library and internet protocol library
#include <sys/socket.h>
#include <netinet/in.h>

int main(){
	
	// Create the server socket
	int server_socket;
	//			domain, type 	    ,protocol
	// domain = AF_INET(IPv4) or AF_INET6			
	server_socket = socket(AF_INET, SOCK_STREAM, 0);

	// define the server address format
	struct sockaddr_in server_address;

	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(9100); // 9002 is choosen arbitrarily. Try other ports if this does not work
	server_address.sin_addr.s_addr = htons(INADDR_ANY);

	bind(server_socket, (struct sockaddr*) &server_address, sizeof(server_address));

	printf("Server is listening for a client!\n");

	listen(server_socket, 128);

	while(1){

		// Integer to hold a potential client socket
		int client_socket;

		client_socket = accept(server_socket, NULL, NULL);

		// send a message from the server
		char server_message[256] = "You have reached the server";
		send(client_socket, server_message, sizeof(server_message), 0);

		//get socket number for client
		int soc_num;
		recvfrom(client_socket, &soc_num, sizeof(soc_num), 0, NULL, NULL);
		printf("Client has joined: %d\n", soc_num);

		//get username from client
		char username[128];
		recvfrom(client_socket, username,  sizeof(username), 0, NULL, NULL); 
		printf("Username: %s\n", username);

		//loop and receive client commands
		while(1){			
			//get the client command
			char client_response[128];
			int ret = recvfrom(client_socket, client_response, 128, 0, NULL, NULL);  
			//if client has no input (exit), end the connection and exit loop
			if(ret <= 0){
				printf("Client connection has ended\n");
				close(client_socket);
				client_socket = -1;
				break;
			}
			//run the command and clear the client response array
			printf("Command: %s\n", client_response);
			//use system to run the command
			//since system is used, don't have to fork and exec
			system(client_response);
			memset(client_response, 0, 128);
		}
	}	

	// Close the socket
	close(server_socket);

	return 0;

}
