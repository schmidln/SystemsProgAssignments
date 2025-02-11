/*
 * Author: Lucas Schmidt
 * Assignment: PA1 - Length Unit Converter
 * Description: This program converts length units including  between metric and US customary systems.
 * Date: February 7th, 2025
 */


#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#define MAX_BUFFER_SIZE 256
#define IN_TO_CM 2.54

typedef enum {
    MM, CM, M, KM,
    IN, FT, YD, MI,
    Unsupported
} Unit;

static const char * const unit_strings[] = { "mm", "cm", "m", "km", "in", "ft", "yd", "mi" };
static const double factors[] = { 0.1, 1.0, 100.0, 100000.0, 1.0, 12.0, 36.0, 63360.0 };
static const int num_unit_strings = sizeof(unit_strings) / sizeof(unit_strings[0]);

int is_metric(const Unit unit) {
    return (unit >= MM && unit <= KM);
}

Unit unit_string_to_unit(const char * const unit_string) {
   for (int i = 0; i < num_unit_strings; i++) {
       if (!strcmp(unit_string, unit_strings[i])) {
           return (Unit)i;
       }
   }
   return Unsupported;
}

double convert(double value, const char * const input_unit_string, const char * const output_unit_string) {
    Unit input_unit = unit_string_to_unit(input_unit_string);
    Unit output_unit = unit_string_to_unit(output_unit_string);

    if (input_unit == Unsupported || output_unit == Unsupported) {
        return NAN;
    }

    if (!is_metric(input_unit) && is_metric(output_unit)) {
        return ((value * factors[input_unit]) * IN_TO_CM) / factors[output_unit];
    }
    else if (is_metric(input_unit) && !is_metric(output_unit)) {
        return ((value * factors[input_unit]) / IN_TO_CM) / factors[output_unit];
    }
    else {
        return value * (factors[input_unit] / factors[output_unit]);
    }
}

int capture_and_scan_input(double *value, char *input_string, char *output_string) {
    printf("> ");
    char user_input[MAX_BUFFER_SIZE];

    if (!fgets(user_input, MAX_BUFFER_SIZE, stdin)) {
        return 0;
    }

    user_input[strcspn(user_input, "\n")] = 0;

    if (sscanf(user_input, "%lf %s %s", value, input_string, output_string) != 3) {
        return 1;
    }

    return 3;
}

void print_allowable_units() {
    printf("Allowable units:");
    for (int i = 0; i < num_unit_strings; i++) {
        printf(" %s", unit_strings[i]);
    }
    printf(".\n");
}

int main() {
    puts("Please enter a length to convert, in the form <number> <input-unit> <output-unit>.");
    puts("Enter any letter to quit.");

    double num;
    char input_unit[100];
    char output_unit[100];

    while (capture_and_scan_input(&num, input_unit, output_unit) == 3 ) {
        double converted_num = convert(num,input_unit,output_unit);
        if (isnan(converted_num)) {
            print_allowable_units();
        }
        else {
            printf("%.6f %s = %.6f %s\n", num, input_unit, converted_num, output_unit);
        }
    }

    return 0;
}
