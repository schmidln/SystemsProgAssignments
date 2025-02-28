/*
 * Authors: Andy Bello - belloac@bc.edu, Lucas Schmidt - schmidln@bc.edu
*/

#include <math.h>
#include <stdio.h>
#include <string.h>

#include "fp_analyzer.h"

int main(int argc, char ** argv) {
    if (argc == 2 && strcmp(argv[1], "special") == 0) {
        FP_TYPE special[] = {1.0/0.0, -1.0/0.0, NAN, -NAN};
	char * strings[] = {"inf", "-inf", "nan", "-nan"};
    	for (int i = 0; i < sizeof(special) / sizeof(special[0]); ++i) {
		printf("%s\n", strings[i]);
		print_components(special[i]);
		printf("\n");
    	}
    } else {
	FP_TYPE input;
	printf("Please enter a floating-point number or q to quit.\n");
	while(1) {
	    printf("> ");
            if (scanf(SPECIFIER, &input) != 1) {
	        break;
	    }
	    printf(SPECIFIER, input);
	    printf("\n");
	    Converter conv;
	    conv.floating_point = input;
            print_components(input);
	    print_reconstitution(input, conv.components);
	}
    }
}
