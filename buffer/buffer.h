#ifndef BUFFER_H
#define BUFFER_H

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define BUFLEN 4096
#define LINELEN 1000
#define HEADER_TERMINATOR "\r\n\r\n"
#define HEADER_TERMINATOR_SIZE (sizeof(HEADER_TERMINATOR) - 1)
#define CONTENT_LENGTH "Content-Length: "
#define CONTENT_LENGTH_SIZE (sizeof(CONTENT_LENGTH) - 1)

typedef struct {
    char *data;
    size_t size;
} buffer;

buffer buffer_init(void);
void buffer_destroy(buffer *buffer);
int buffer_is_empty(buffer *buffer);
void buffer_add(buffer *buffer, const char *data, size_t data_size);
int buffer_find(buffer *buffer, const char *data, size_t data_size);
int buffer_find_insensitive(buffer *buffer, const char *data, size_t data_size);
void compute_message(char *message, const char *line);
#endif
