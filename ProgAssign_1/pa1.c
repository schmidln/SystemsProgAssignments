#include <stdio.h>
#include <ctype.h>
#include <string.h>

int capture_and_scan_input(double * value, char * input_string, char * output_string) {

    
}

int main() {
    puts("Please enter a length to convert, in the form <number> <input-unit> <output-unit>.");
    puts("Enter any letter to quit.");
    char user_input[100];
    while (1) {
        printf("> ");
        fgets(user_input, 100, stdin);
        user_input[strcspn(user_input, "\n")] = 0;
        if (strlen(user_input) == 1 && isalpha(user_input[0])) {
            break;
        }
        else {
            puts(user_input);
        }
    }
    return 0;
}
