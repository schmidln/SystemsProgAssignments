/* 
Authors: 
Lucas Schmidt - schmidln@bc.edu
Andy Bello - belloac@bc.edu
*/

int show_hidden = 0;

int main(int argc, char **argv) {
    if (argc == 1) {
        show_hidden = 0;
    }
    
    else if (argc == 2) {
        if (argv[1] == "-l") {
            show_hidden = 1;
        }
        else {
            return 1;
        }

    }
    else {
        return 1;
    }
}