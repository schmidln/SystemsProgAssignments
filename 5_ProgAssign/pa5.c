/* 
Authors: 
Lucas Schmidt - schmidln@bc.edu
Andy Bello - belloac@bc.edu
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int show_hidden = 0;

void walk_dir(char *dir, int indent) {
    // To be implemented
}

int main(int argc, char **argv) {
    if (argc == 2) {
        if (strcmp(argv[1], "-l") == 0) {
            // "-l" is not valid by itself
            return EXIT_FAILURE;
        } else {
            walk_dir(argv[1], 0);
            return EXIT_SUCCESS;
        }
    }

    else if (argc == 3) {
        if (strcmp(argv[2], "-l") == 0) {
            show_hidden = 1;
            walk_dir(argv[1], 0);
            return EXIT_SUCCESS;
        } else {
            return EXIT_FAILURE;
        }
    }

    else {
        // Either no args or too many
        return EXIT_FAILURE;
    }
}
