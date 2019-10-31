// Note: I recommend implementing the tcpserver.c first.
//
// Compile with: gcc tcpclient.c -o tcpclient
//
// Implement tcp client below.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <unistd.h>
#include <netinet/in.h>

int main(){

	// Create a socket
	int client_socket;
	// Note that the protocol matches our server here
	// Also note I am doing some error checking for my socket.
	// You may consider updating your server code to do the same.
	if((client_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		printf("\n Socket creation error \n");
	}

	// Our client will connect to a server with the following address and port.
	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET; // 32-bit IPV4 address
	server_address.sin_port = htons(9100); // convert to the appropriate port number using htons
	server_address.sin_addr.s_addr = htons(INADDR_ANY); // Our ip address, which is localhost using INADDR_ANY

	// Attempt to connect
	// Returns an integer as an error code (zero means everything worked)
	int connection_status;

	// Report back to user if our connection was successful
	if( (connection_status = connect(client_socket, (struct sockaddr*)&server_address, sizeof(server_address))) <0 ){
		printf("%d Connection unsuccessful\n", connection_status);
		close(client_socket);
		exit(0);	
	}else{
		printf("Connected successfully\n");
	}

	// Receive data from the server
	char server_response[256];
	recv(client_socket, &server_response, sizeof(server_response), 0);

	// Print out the server's response
	printf("The server sent the data: %s\n",server_response);

	//send client socker number to server
	sendto(client_socket, &client_socket, sizeof(client_socket), 0, (struct sockaddr*)&server_address, sizeof(server_address)); 

	char client_input[128];
	char username[128];

	//get username and remove any leading new line character	
	printf("Please enter your username: ");
	fgets(username, 128, stdin);
	if ((strlen(username) > 0) && (username[strlen(username) - 1] == '\n')){
		username[strlen(username) - 1] = '\0';
	}
	
	//send username to server
	sendto(client_socket, username, sizeof(username), 0, (struct sockaddr*)&server_address, sizeof(server_address)); 

	//loop indefinitely allow user to input commands
	while(1){

		printf("client> ");
		
		//take user input and remove ending new line character		
		fgets(client_input, 128, stdin);
		if ((strlen(client_input) > 0) && (client_input[strlen(client_input) - 1] == '\n')){
        		client_input[strlen(client_input) - 1] = '\0';
		}
		
		//if user enters exit, exit the loop
		if(strcmp(client_input, "exit") == 0){
			break;
		}
		
		//create a char array of exact size of input, so you dont extra data
		//and send server exact data
		char subclient[strlen(client_input)];
		memcpy(subclient, client_input, strlen(client_input));
		int ret = sendto(client_socket, subclient, sizeof(subclient), 0, (struct sockaddr*)&server_address, sizeof(server_address));
	}


	// Then close the socket
	close(client_socket);

	return 0;

}
