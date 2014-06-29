// gcc -Wall -O2 -o socket_client socket_client.c
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char** argv) {
	int connection;
	ssize_t transmitted;
	struct addrinfo hints;
	struct addrinfo *addrs;
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
	
	if ((connection = socket(addrs->ai_family, addrs->ai_socktype, addrs->ai_protocol)) < 0) {
		perror("socket: ");
		return -1;
	}
	
	if (connect(connection, addrs->ai_addr, addrs->ai_addrlen) < 0) {
		perror("Connect failed: ");
		return -1;
	}
	
	transmitted = send(connection, message, sizeof(message), 0);
	if (transmitted != (ssize_t)sizeof(message)) {
		printf("Error during transmission\n");
	}
	printf("Sended: %s\n", message);
	
	transmitted = recv(connection, buffer, sizeof(buffer) - 1, 0);
	buffer[transmitted] = '\0';	
	printf("Received: %s\n", buffer);
	
	close(connection);
	freeaddrinfo(addrs);
	
	return 0;
}
