#include "circular_buffer.h"
#include <math.h>

circular_buffer_t *new_circular_buffer(size_t capacity)
{
    capacity  = capacity;
    circular_buffer_t* buffer = (circular_buffer_t*)malloc(sizeof(circular_buffer_t));
    buffer->capacity = capacity;
    buffer->reader = 0;
    buffer->writer = 0;
    buffer->length = 0;
    buffer->values = (buffer_value_t*)malloc(sizeof(buffer_value_t) * capacity);
    return buffer;
}

void delete_buffer(circular_buffer_t *buffer)
{
    if (buffer)
    {
        if (buffer->values)
        {
            free(buffer->values);
            buffer->values = NULL;
        }

        free(buffer);
    }
}

void clear_buffer(circular_buffer_t *buffer)
{
    buffer->length = 0;
}

int16_t overwrite(circular_buffer_t *buffer, buffer_value_t value)
{
    if (buffer && buffer->capacity == buffer->length)
    {
        buffer->values[buffer->reader] = value;
        buffer->reader = buffer->writer = (buffer->reader + 1) % buffer->capacity;
        return EXIT_SUCCESS;
    }

    return write(buffer, value);
}

int16_t write(circular_buffer_t *buffer, buffer_value_t value)
{
    if (buffer && buffer->capacity > buffer->length)
    {
        buffer->values[buffer->writer] = value;
        buffer->writer = (buffer->writer + 1) % buffer->capacity;
        buffer->length++;
        return EXIT_SUCCESS;
    }

    errno = ENOBUFS;
    return EXIT_FAILURE;
}

int16_t read(circular_buffer_t *buffer, buffer_value_t *value)
{
    if (buffer && buffer->length > 0)
    {
        *value = buffer->values[buffer->reader];
        buffer->reader = (buffer->reader + 1) % buffer->capacity;
        buffer->length--;
        return EXIT_SUCCESS;
    }

    errno = ENODATA;
    return EXIT_FAILURE;
}
