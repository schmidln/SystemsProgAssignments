/*
 * Authors: Lucas Schmidt and Andy Bello
 * Assignment: PA2 - Visualizing the Central Limit Theorem
 * Description: This program creates a graphical representation of the central limit theorem.
 * Date: February 14th, 2025
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define SAMPLES 1000
#define RUNS 10
#define HISTOGRAM_SPAN 2.0
#define BINS 10
#define SCALE 1

double get_mean_of_uniform_random_samples() {
    double sum = 0;

    for (int i = 0; i < SAMPLES; ++i) {
        sum += rand();
    }

    double rand_average = sum / SAMPLES;
    double in_range_value = rand_average / RAND_MAX;
    in_range_value *= 2.0;
    in_range_value -= 1.0;
    return in_range_value;
}

double populate_values_and_get_mean(double *values) {
    double sum = 0;

    for (int i = 0; i < RUNS; ++i) {
        values[i] = get_mean_of_uniform_random_samples();
        sum += values[i];
    }

    return sum / RUNS;
}

double get_mean_squared_error(double *values, double mean) {
    double error;
    double sum = 0;
    for (int i = 0; i < RUNS; ++i) {
        error = values[i] - mean;
        error *= error;
        sum += error;
    }

    return sum / RUNS;
}

void create_histogram(double *values, int *counts) {
    memset(counts, 0, BINS * sizeof(int));

    double bin_size = HISTOGRAM_SPAN / (double)BINS;
    double min_value = -HISTOGRAM_SPAN / 2;

    for (int i = 0; i < RUNS; ++i) {
        int bin_index = (int)((values[i] - min_value) / bin_size);

        if (bin_index < 0) {
            bin_index = 0;
        } else if (bin_index >= BINS) {
            bin_index = BINS - 1;
        }

        counts[bin_index]++;
    }
}

void print_histogram(int *counts) {
    double bin_size = HISTOGRAM_SPAN / (double)BINS;
    double bin_start = -HISTOGRAM_SPAN / 2;

    for (int i = 0; i < BINS; ++i) {
        double bin_label = bin_start + bin_size / 2;
        int num_xs = counts[i] / SCALE;

        printf("%6.3f | ", bin_label);
        
        for (int j = 0; j < num_xs; ++j) {
            printf("X");
        }
        printf("\n");

        bin_start += bin_size;
    }
}

int main() {
    srand(time(NULL));

    double *values = (double *)calloc(RUNS, sizeof(double));
    int counts[BINS];

    double mean = populate_values_and_get_mean(values);
    printf("Mean: %lf\n", mean);
    printf("Mean squared error: %lf\n", get_mean_squared_error(values, mean));

    create_histogram(values, counts);
    print_histogram(counts);

    free(values);
    return 0;
}

