#define PAGE_SIZE 4096
#define WORD_SIZE 8
#define SUCCESS 0
#define FAILURE -1

typedef struct Header {
    size_t size;
    Header * next;
    Header * previous;
} Header;