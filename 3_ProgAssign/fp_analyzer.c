/*
 * Authors: Andy Bello - belloac@bc.edu, Lucas Schmidt - schmidln@bc.edu
*/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "fp_analyzer.h"

void print_bits(UINT_TYPE value, int bit_count) {
    for (int i = bit_count - 1; i >= 0; i--) {
        putchar((value & ((UINT_TYPE)1 << i)) ? '1' : '0');
    }
}

void print_components(FP_TYPE value) {
    Converter conv;
    conv.floating_point = value;
    printf("Bits: ");
    print_bits(conv.components.sign, SIGN_BITS);
    printf("|");
    print_bits(conv.components.exponent, EXPONENT_BITS);
    printf("|");
    print_bits(conv.components.mantissa, MANTISSA_BITS);
    printf("\n");
}

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

void print_normalized(FP_TYPE value, Components comp) {
    printf("Original value:\n");
    FP_TYPE mantissa = ((FP_TYPE)comp.mantissa / power_of_2(MANTISSA_BITS));
    printf("(-1)^{%d} x (1 + ", (int)comp.sign);
    printf(SPECIFIER, mantissa);
    printf(") x 2^{%d - %d}\n", (int)comp.exponent, BIAS);
    int true_exponent = (int)comp.exponent - BIAS;
    mantissa += 1;
    int negative_one_power;
    if ((int)comp.sign == 0) {
        negative_one_power = 1;
    } else {
        negative_one_power = -1;
    }
    printf("  = %d x ", negative_one_power);
    printf(SPECIFIER, mantissa);
    printf(" x 2^{%d}\n", true_exponent);
    mantissa *= negative_one_power;
    printf("  = ");
    printf(SPECIFIER, mantissa);
    printf(" x %.0f\n", power_of_2(true_exponent));
    FP_TYPE reconstructed = mantissa * power_of_2(true_exponent);
    printf("  = %.45f\n", reconstructed);
}

void print_denormalized(FP_TYPE value, Components comp) {
    if (comp.exponent == 0 && comp.mantissa == 0) {
	if (comp.sign == 1) {
	    printf("Original value: -0.0\n");
	} else {
	    printf("Original value: 0.0\n");
	}
    } else {
        printf("Original value:\n");
	FP_TYPE mantissa = ((FP_TYPE)comp.mantissa / power_of_2(MANTISSA_BITS));
	printf("(-1)^{%d} x ", (int)comp.sign);
        printf("%.45f", mantissa);
	printf(" x 2^{1 - %d}\n", BIAS);
	int negative_one_power;
	if (comp.sign == 0) {
	    negative_one_power = 1;
	} else {
	    negative_one_power = -1;
	}
	printf("  = %d x ", negative_one_power);
	printf(SPECIFIER, mantissa);
	printf(" x 2^{%d}\n", 1 - BIAS);
	mantissa *= negative_one_power;
	FP_TYPE two_raised_to_abs_bias =  power_of_2(BIAS - 1);
	printf("  = ");
	printf("%.45f", mantissa);
	printf(" x 1/%.0f\n", two_raised_to_abs_bias);
	FP_TYPE reconstructed = mantissa * power_of_2(1 - BIAS);
        printf("  = %.45f\n", reconstructed);
    }
}

void print_reconstitution(FP_TYPE value, Components comp) {
    if (comp.exponent == 0) {
        print_denormalized(value, comp);
    } else {
        print_normalized(value, comp);
    }
}
