#include "fp_analyzer.h"

// Function to print bits from left to right
void print_bits(UINT_TYPE value, int bit_count) {
    for (int i = bit_count - 1; i >= 0; i--) {
        putchar((value & ((UINT_TYPE)1 << i)) ? '1' : '0');
    }
}

// Function to print components of a floating-point number
void print_components(FP_TYPE value) {
    Converter conv;
    conv.floating_point = value;
    
    printf("%g -> ", value);
    print_bits(conv.integer_representation, sizeof(FP_TYPE) * 8);
    printf("\n");
}

// Function to compute power of 2 without using pow()
FP_TYPE power_of_2(int exponent) {
    FP_TYPE result = 1.0;
    if (exponent >= 0) {
        for (int i = 0; i < exponent; i++) {
            result *= 2.0;
        }
    } else {
        for (int i = 0; i < -exponent; i++) {
            result /= 2.0;
        }
    }
    return result;
}

// Function to print normalized floating-point representation
void print_normalized(FP_TYPE value, Components comp) {
    FP_TYPE mantissa = 1.0 + ((FP_TYPE)comp.mantissa / power_of_2(MANTISSA_BITS));
    int true_exponent = (int)comp.exponent - BIAS;
    FP_TYPE reconstructed = mantissa * power_of_2(true_exponent);
    printf("Reconstructed: %.45f\n", reconstructed);
}

// Function to print denormalized floating-point representation
void print_denormalized(FP_TYPE value, Components comp) {
    if (comp.exponent == 0 && comp.mantissa == 0) {
        printf("Zero\n");
    } else {
        FP_TYPE mantissa = ((FP_TYPE)comp.mantissa / power_of_2(MANTISSA_BITS));
        FP_TYPE reconstructed = mantissa * power_of_2(1 - BIAS);
        printf("Reconstructed: %.45f\n", reconstructed);
    }
}

// Function to determine and print reconstitution
void print_reconstitution(FP_TYPE value, Components comp) {
    if (comp.exponent == 0) {
        print_denormalized(value, comp);
    } else {
        print_normalized(value, comp);
    }
}
