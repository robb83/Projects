#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include "file.h"

int read_content(Buffer *buffer, char* filename)
{
	int fd;
	
	buffer->length = 0;
	buffer->content = NULL;
	
	if ((fd = open(filename, O_RDONLY)) < 0)
	{
		return 0;
	}
	
	int length = lseek(fd, 0, SEEK_END);
	if (length == -1)
	{
		return 0;
	}
	lseek(fd, 0, SEEK_SET);
	
	buffer->length = length;
	buffer->content = (char*)malloc(length);
	int readed = read(fd, buffer->content, length);
	
	close(fd);
	
	return readed;
}

int write_content(Buffer *buffer, char* filename)
{
	int fd;
	
	if (buffer == NULL || filename == NULL)
	{
		return 0;
	}
	
	if ((fd = open(filename, O_WRONLY)) < 0)
	{
		return 0;
	}
	
	int writed = write(fd, buffer->content, buffer->length);
	
	close(fd);
	
	return writed;
}

int buffer_clear(Buffer* buffer)
{
	if (buffer == NULL || buffer->content == NULL || buffer->length == 0) 
	{
		return 0;
	}
	
	memset(buffer->content, 0, buffer->length);
	
	return 1;
}

int buffer_alloc(Buffer* buffer, int length)
{
	if (buffer == NULL || buffer->content == NULL)
	{
		return 0;
	}
	
	buffer->content = (char*)malloc(length);
	if (buffer->content == NULL)
	{
		buffer->length = length;
		return 1;
	} else {
		buffer->length = 0;
		return 0;
	}
}

int buffer_free(Buffer* buffer)
{
	if (buffer == NULL || buffer->content == NULL)
	{
		return 0;
	}
	
	free(buffer->content);
	buffer->content = NULL;
	buffer->length = 0;
	
	return 1;
}
