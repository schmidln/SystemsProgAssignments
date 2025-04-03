/* 
Authors: Lucas Schmidt - schmidln@bc.edu, Andy Bello - belloac@bc.edu
*/

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int show_hidden = 0;

int is_pwd_or_parent(char *filename) {
    return (strcmp(filename, ".") == 0 || strcmp(filename, "..") == 0);
}

void indent(int indent) {
    for (int i = 0; i < indent * 4; i++) {
        putchar(' ');
    }
}

void walk_dir(char * dir, int indent_level) {
    DIR * dp = opendir(dir);
    if (dp == NULL) {
        return;
    }
    struct dirent * entry;
    while ((entry = readdir(dp)) != NULL) {
        if (is_pwd_or_parent(entry->d_name)) {
            continue;
        } 
        if (entry->d_name[0] == 46 && show_hidden == 0) {
            continue;
        } 
        if (entry->d_type == DT_DIR) {
            indent(indent_level);
            printf("%s", entry->d_name);
            putchar(':');
            putchar('\n');
            size_t buffer_length = strlen(dir) + 1 + strlen(entry->d_name) + 1;
            char buffer[buffer_length];
            snprintf(buffer, buffer_length, "%s/%s", dir, entry->d_name);
            walk_dir(buffer, indent_level + 1); 
        } else {
            indent(indent_level);
            printf("%s\n", entry->d_name);   
        }
    }
    closedir(dp);
}

int main(int argc, char ** argv) {
    if (argc == 2) {
        walk_dir(argv[1], 0);
        return EXIT_SUCCESS;
    } else if (argc == 3) {
        if (strcmp(argv[2], "-l") == 0) {
            show_hidden = 1;
            walk_dir(argv[1], 0);
            return EXIT_SUCCESS;
        } else {
            return EXIT_FAILURE;
        }
    } else {
        return EXIT_FAILURE;
    }
}