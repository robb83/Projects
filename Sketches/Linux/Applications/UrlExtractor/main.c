// gcc -Wall -O2 -o urlextractor main.c
// TODOS:
//	 multi thread in directory search mode
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <errno.h>
#include <dirent.h>

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

	// reset value for new search
	position = 0;
	limit = 0;
	endOfFile = 0;
	url_position = 0;

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
								push(current);
							
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

char* combine(char* s1, char* s2) {
	int len = strlen(s1) + strlen(s2) + 2 /* separator + string terminateing byte  */;
	
	// allocating memory for new string
	char* result = (char*)malloc(sizeof(char) * len);
	if (result == NULL) {
		// not enought memory
		return NULL;
	}
	
	char* pos = result;
	// copy first part
	pos = stpcpy(pos, s1);
	
	// append separator if needed
	if (result < pos && *(pos - 1) != '/') {
		*pos++ = '/';
		*pos = '\0';
	}
	
	// copy second part
	pos = stpcpy(pos, s2);
	
	return result;
}

void searchInFile(char* filename) {
	if (!filename) return;
	
	fd = open(filename, O_RDONLY);
	if (fd == -1) {
		fprintf(stderr, "Can't open file: %s (%s)\n", strerror(errno), filename);
		return;
	}
	search();
	close(fd);
}

void searchInDirectory(char* directory) {
	if (!directory) return;
	struct stat s;
	DIR *d;
	struct dirent *de;
	
	d = opendir(directory);
	if (d == NULL) {
		fprintf(stderr, "Can't open directory: %s (%s)\n", strerror(errno), directory);
		return;
	}
	
	while ((de = readdir(d)) != NULL) {
		if (strcmp(de->d_name, ".") == 0 || strcmp(de->d_name, "..") == 0)
			continue;
		
		char* fullpath = combine(directory, de->d_name);
		if (fullpath == NULL) {
			fprintf(stderr, "Not enough memory\n");
			break;
		}
		
		if (stat(fullpath, &s) == -1) {
			free(fullpath);
			break;
		}

		if (S_ISDIR(s.st_mode)) {
			searchInDirectory(fullpath); 
		} else if (S_ISREG(s.st_mode)) {
			searchInFile(fullpath); 
		}
		
		free(fullpath);
	}
	
	closedir(d);
}

void searchInInput() {
	fd = 0;
	search();
	close(fd);
}

int main(int argc, char** argv) {
	char* directory = NULL;
	char* filename = NULL;
	int c;
	
	while((c = getopt(argc, argv, "f:d:h")) != -1) {
		switch(c) {
			case 'd':
				directory = optarg;
				break;
			case 'f':
				filename = optarg;
				break;
			case '?':
			case 'h':
				printf("Usage: -f file or -d directory\n");
				return -1;
		}
	}

	if (directory == NULL && filename == NULL) {
		searchInInput();
		return 0;
	}
	
	if (directory != NULL) {
		searchInDirectory(directory);
	}
	
	if (filename != NULL) {
		searchInFile(filename);
	}
	
	return 0;
}
