#include <stddef.h>
#include <stdio.h>
#include <sys/mman.h>

#define PAGE_SIZE 4096
#define WORD_SIZE 8
#define SUCCESS 0
#define FAILURE -1

typedef struct Header {
    size_t size;
    struct Header * next;
    struct Header * previous;
} Header;

void * mem_alloc(size_t requested_size);
void mem_free(void * ptr);
