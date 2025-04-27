/*
 * Author: Amittai Aviram - aviram@bc.edu
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mem_alloc.h"

// Debugging function forward declarations:
Header * get_header(void * mem);
void print_header(Header * header);
void print_list();


int main() {
    print_list();
    return EXIT_SUCCESS;
}

