/*
 * Authors: Andy Bello - belloac@bc.edu, Lucas Schmidt - schmidln@bc.edu
*/ 

#include "mem_alloc.h"

Header * free_list = NULL;

int is_allocated(Header * header) {
    return header->size & 1;
}

int is_free(Header * header) {
    return !is_allocated(header);
}

void set_allocated(Header * header) {
    header->size = header->size | 1;
}

void set_free(Header * header) {
    header->size = header->size >> 1;
    header->size = header->size << 1;
}

Header * get_header(void * mem) {
    return (Header *)((char *)mem - sizeof(Header));
}

int same_page(Header * h1, Header * h2) {
    return ((size_t)h1 & ~0xFFF) == ((size_t)h2 & ~0xFFF);
}

int mem_init() {
    Header * new_page = (Header *)mmap(NULL, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
    if (new_page == MAP_FAILED) {
        return FAILURE;
    }
    new_page->size = PAGE_SIZE - sizeof(Header);
    new_page->next = NULL;
    new_page->previous = NULL;
    free_list = new_page;
    return SUCCESS;
}

int mem_extend(Header * last){
    Header * new_page = (Header *)mmap(NULL, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
    if (new_page == MAP_FAILED) {
        return FAILURE;
    }
    new_page->size = PAGE_SIZE - sizeof(Header);
    new_page->next = NULL;
    last->next = new_page;
    new_page->previous = last;
    return SUCCESS;
}

void * mem_alloc(size_t requested_size) {
    if (requested_size > PAGE_SIZE - sizeof(Header)) {
        return NULL;
    }
    if (free_list == NULL) {
	int result = mem_init();
        if (result == FAILURE) {
	    return NULL;
	}
    }
    size_t aligned_size;
    if (requested_size % WORD_SIZE == 0) {
        aligned_size = requested_size;
    } else {
	aligned_size = requested_size + (WORD_SIZE - (requested_size % WORD_SIZE));
    }
    Header * current = free_list;
    while (current->next != NULL && (is_allocated(current) || current->size < aligned_size)) {
        current = current->next;
    }
    if (is_allocated(current) || current->size < aligned_size){
	int extend_result = mem_extend(current);
        if (extend_result == FAILURE) {
            return NULL;
        }
	current = current->next;
    }
    if (current->size > (aligned_size + sizeof(Header))) {
	Header * new_header = (Header *)((char *)current + sizeof(Header) + aligned_size);
	new_header->size = current->size - aligned_size - sizeof(Header);
	current->size = aligned_size;
	new_header->next = current->next;
	current->next = new_header;
	new_header->previous = current;
	if (new_header->next != NULL) {
	    new_header->next->previous = new_header;
	}
    }
    set_allocated(current);
    return (char *)current + sizeof(Header);
}

void mem_free(void * ptr) {
    Header * header = get_header(ptr);
    set_free(header);
    Header * next = header->next;
    if (next != NULL && is_free(next) && same_page(header, next)) {
        header->size += (next->size + sizeof(Header));
        header->next = next->next; 
        if (header->next != NULL) {
            header->next->previous = header;
        }
    }
    Header * previous = header->previous;
    if ((previous != NULL && is_free(previous) && same_page(header, previous))) {
        header = previous;
        header->size += (header->next->size + sizeof(Header));
        header->next = header->next->next;
        if (header->next != NULL) {
            header->next->previous = header;
        }
    }
    if (header->size == PAGE_SIZE - sizeof(Header)) {
        if (header->previous != NULL) {
            header->previous->next = header->next;
            if (header->next != NULL) {
                 header->next->previous = header->previous;
            }   
        } else if ((header == free_list) && header->next != NULL) {
            free_list = header->next;
            free_list->previous = NULL;
        } else {
            free_list = NULL;
        }
        munmap(header, PAGE_SIZE);
    }
}

void print_list() {
    Header * current = free_list;
    if (current == NULL) {
        printf("(Empty list.)\n");
	return;
    }
    while (current != NULL) {
        printf("%p -> ", (void *)current);
	current = current->next;
    }
    putchar('\n');
}

size_t get_size(Header * header) {
    return header->size & ~1;
}

void print_header(Header * header) {
    printf("	Addr: %p\n", (void *)header);
    printf("	Size: %zu\n", get_size(header));
    if (header->previous == NULL) {
        printf("    Previous: 0x0\n");
    } else {
	printf("    Previous: %p\n", (void *)header->previous);
    }
    printf("	Next: %p\n", (void *)header->next);
}
