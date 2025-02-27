#ifndef FP_ANALYZER_H
#define FP_ANALYZER_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Conditional compilation to handle float and double versions
#ifdef DOUBLE
    #define FP_TYPE double
    #define UINT_TYPE unsigned long
    #define EXPONENT_BITS 11
    #define MANTISSA_BITS 52
    #define BIAS 1023
#else
    #define FP_TYPE float
    #define UINT_TYPE unsigned int
    #define EXPONENT_BITS 8
    #define MANTISSA_BITS 23
    #define BIAS 127
#endif

// Structure to store components of floating-point representation
typedef struct {
    UINT_TYPE mantissa : MANTISSA_BITS;
    UINT_TYPE exponent : EXPONENT_BITS;
    UINT_TYPE sign : 1;
} Components;

// Union to facilitate conversion between floating-point and integer representation
typedef union {
    FP_TYPE floating_point;
    UINT_TYPE integer_representation;
    Components components;
} Converter;

// Function prototypes
void print_bits(UINT_TYPE value, int bit_count);
void print_components(FP_TYPE value);
void print_normalized(FP_TYPE value, Components comp);
void print_denormalized(FP_TYPE value, Components comp);
void print_reconstitution(FP_TYPE value, Components comp);
FP_TYPE power_of_2(int exponent);

#endif // FP_ANALYZER_H
