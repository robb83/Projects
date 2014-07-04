// gcc -Wall -O2 -o urlextractor main.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>

#define whitespace(ch) (current <= ' ')

static int fd;
static char buffer[1024];
static char url[1024];
static int position = 0;
static int limit = 0;
static int endOfFile = 0;
static int url_position = 0;

char pop() {
	if (endOfFile) 
		return -1;
	
	if (position >= limit) {
		int readed = read(fd, buffer, sizeof(buffer));
		if (readed == 0) {
			endOfFile = 1;
			return -1;
		}
		
		position = 0;
		limit = readed;		
	}
	
	return buffer[position++];
}

void push(char current) {
	if (url_position < sizeof(url) - 1)
		url[url_position++] = current;
}

void writeUntilWhitespace() {
	
	char current;
	
	while(1) {
		current = pop();
		
		if (current == -1 || whitespace(current))
			break;
			
		push(current);
	}
	
	url[url_position] = '\0';
}

void search() {
	char current;
	
	while(1) {
		current = pop();
		
begin:
		if (current == -1)
			break;
		
		if (current == 'h' || current == 'H') {
			url_position = 0;	
			push(current);			
			current = pop();
			
			if (current == 't' || current == 'T') {
				
				push(current);			
				current = pop();
				if (current == 't' || current == 'T') {
					
					push(current);			
					current = pop();
					if (current == 'p' || current == 'P') {
						
						push(current);			
						current = pop();
						if (current == ':') {
							
							push(current);			
							current = pop();
							if (current == '/') {
			
								push(current);			
								current = pop();
								if (current == '/') {

									writeUntilWhitespace();
									printf("%s\n", url);
									
								} else {
									goto begin;
								}
								
							} else {
								goto begin;
							}
							
						} else {
							goto begin;
						}
						
					} else {
						goto begin;
					}
					
				} else {
					goto begin;
				}
				
			} else {
				goto begin;
			}
		} else if (current == 'f' || current == 'F') {
			url_position = 0;
			push(current);			
			current = pop();
			
			if (current == 't' || current == 'T') {
				
				push(current);			
				current = pop();
				if (current == 'p' || current == 'P') {
					
					push(current);			
					current = pop();
					if (current == ':') {
							
						push(current);			
						current = pop();
						if (current == '/') {
			
							push(current);			
							current = pop();
							if (current == '/') {

								writeUntilWhitespace();
								printf("%s\n", url);
									
							} else {
								goto begin;
							}
								
						} else {
							goto begin;
						}
						
					} else {
						goto begin;
					}
				} else {
					goto begin;
				}
			} else {
				goto begin;
			}
		}
	}
}

int main(int argc, char** argv) {
	if (argc < 2) {
		printf("Usage: filename\n");
		return -1;
	}
	
	fd = open(argv[1], O_RDONLY);
	if (fd == -1) {
		perror("Can't open file: ");
		return -1;
	}

	search();
	
	close(fd);	
	return 0;
}
