typedef struct {
	int length;
	char* content;
} Buffer;

int read_content(Buffer *buffer, char* filename);
int write_content(Buffer *buffer, char* filename);
int buffer_free(Buffer* buffer);
int buffer_alloc(Buffer* buffer, int length);
int buffer_clear(Buffer* buffer);