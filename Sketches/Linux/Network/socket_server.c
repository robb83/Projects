// gcc -Wall -O2 -o socket_server socket_server.c
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char** argv) {
	int server, client;
	ssize_t transmitted;
	struct sockaddr_in client_addr;
	socklen_t client_addrlen = sizeof(struct sockaddr);
	struct addrinfo hints, *addrs;
	char message[] = "Hello World";
	char buffer[256];
	
	if (argc != 3) {
		printf("usage: hostname port\n");
		return -1;
	}
	
	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
   
	if (getaddrinfo(argv[1], argv[2], &hints, &addrs) != 0) {
		perror("Can't resolve address: ");
		return -1;
	}
	
	if ((server = socket(addrs->ai_family, addrs->ai_socktype, addrs->ai_protocol)) < 0) {
		perror("socket: ");
		return -1;
	}
	
	if (bind(server, addrs->ai_addr, addrs->ai_addrlen) < 0) {
		perror("Bind failed: ");
		return -1;
	}
	
	if (listen(server, 1) < 0) {
		perror("Bind failed: ");
		return -1;
	}
	
	if ((client = accept(server, (struct sockaddr*)(&client_addr), &client_addrlen)) < 0) {
		perror("Accept failed: ");
		return -1;
	}
	
	printf("Connected from: %s:%d, Family: %d (%d)\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), client_addr.sin_family, client_addrlen);
	
	transmitted = recv(client, buffer, sizeof(buffer) - 1, 0);
	buffer[transmitted] = '\0';	
	printf("Received: %s\n", buffer);
	
	transmitted = send(client, message, sizeof(message), 0);
	if (transmitted != (ssize_t)sizeof(message)) {
		printf("Error during transmission\n");
	}
	printf("Sended: %s\n", message);
		
	close(server);
	freeaddrinfo(addrs);
	
	return 0;
}
