#include <stdio.h>
#include <ctype.h>
#include <string.h>

int capture_and_scan_input(double *value, char *input_string, char *output_string) {
    printf("> ");
    char user_input[100];

    fgets(user_input, 100, stdin);
    user_input[strcspn(user_input, "\n")] = 0;

    if (strlen(user_input) == 1 && isalpha(user_input[0])) {
        return 1;
    }
    else {
        puts(user_input);
        return sscanf(user_input,"%lf %s %s",value,input_string,output_string);
    }
}

int main() {
    puts("Please enter a length to convert, in the form <number> <input-unit> <output-unit>.");
    puts("Enter any letter to quit.");

    double num;
    char input_unit[100];
    char output_unit[100];

    while (capture_and_scan_input(&num, input_unit, output_unit) == 3 ) {
        printf("%lf\n", num);
        puts(input_unit);
        puts(output_unit);
    }

    return 0;
}

