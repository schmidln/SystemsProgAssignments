Header * free_list = NULL;

// Allocates first page using mmap, assigning to free list. Set free_list header.(header not included in size) Return SUCCESS or FAILURE
int mem_init() {
}

//Extends the free list with a new page when it cannot find a suitable block. Then adds it to the free list.
int mem_extend(Header * last){
}

//If empty, call mem_init
//Rounds the requested size up to the nearest multiple of the word size and looks for big enough block
//If block is bigger than needed, we break it up and create a new header. Then, connect this to the free list.
//Set last bit of size field to show block has been allocated.
//Return the start address of the payload.
//If there is no suitable block, call mem_extend.
void * mem_alloc(size_t requested_size) {
}

//Find its header by looking back sizeof(Header) from the pointer passed into mem_free. clear the lowest order bit of size to show it is unallocated.
//Need to coalesce blocks that are within the same page number.
//Must combine blocks if the next one is free and fixing pointers.
//If the entire page is free, the page should be munmapped and removed from linked list. If there is only one page left and it is a single free block, set free_list to NULL.
void mem_free(void * ptr) {
}
