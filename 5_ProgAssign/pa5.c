/* 
Authors: 
Lucas Schmidt - schmidln@bc.edu
Andy Bello - belloac@bc.edu
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

int show_hidden = 0;

int is_pwd_or_parent(char * filename) {
   if ((filename[0] == 46 && (sizeof(filename)/sizeof(filename[0]) == 1)) || (filename[0] == 46 && filename[1] == 46 && (sizeof(filename)/sizeof(filename[0]) == 2))){
       return 1;
   }
   return 0;
}

void walk_dir(char * dir, int indent) {
    DIR * dp = opendir(dir);
    if (dp == NULL) {
        perror("opendir");
        return;
    }

    struct dirent * entry;

    while ((entry = readdir(dp)) != NULL) {
        if (is_pwd_or_parent(entry -> d_name)) {
            continue;
        }


}

int main(int argc, char ** argv) {
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
        return EXIT_FAILURE;
    }
}
