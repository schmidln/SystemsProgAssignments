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

int is_pwd_or_parent(char *filename) {
    return (strcmp(filename, ".") == 0 || strcmp(filename, "..") == 0);
}

void indent(int indent) {
    for (int i = 0; i < indent; i++) {
        putchar(' ');
        putchar(' ');
        putchar(' ');
        putchar(' ');
    }
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
        if (entry -> d_name[0] == 46 && show_hidden == 0) {
            continue;
        }
        if (entry -> d_type == DT_DIR) {
            printf("%s", entry -> d_name);
            putchar(':');
            putchar('\n');
            walk_dir(entry -> d_name, indent + 1);
        }
        indent(indent);
        printf("%s\n", entry -> d_name);
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
