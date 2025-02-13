#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SAMPLES 1000
#define RUNS 10

double get_mean_of_uniform_random_samples() {
    double sum = 0;

    for (int i = 0; i < SAMPLES; ++i) {
        sum += rand();
    }

    double rand_average  = sum / SAMPLES;
    double in_range_value = rand_average / RAND_MAX;
    in_range_value *= 2.0;
    in_range_value -= 1.0;
    return in_range_value;
}

double populate_values_and_get_mean(double * values) {
    double sum = 0;

    for (int i = 0; i < RUNS; ++i) {
    	values[i] =  get_mean_of_uniform_random_samples();
	sum += values[i];
    }

    double mean_over_sample_averages = sum / RUNS;
    return mean_over_sample_averages;
}

double get_mean_squared_error(double * values, double mean) {
    double error;
    double sum = 0;
    for (int i = 0; i < RUNS; ++i) {
        error = values[i] - mean;
	error *= error;
	sum += error;
    }
    
    return sum / RUNS;    
}

int main() {
    srand(time(NULL));
    double * values = (double *) calloc(RUNS, sizeof(double));
    printf("Mean: %lf\n", populate_values_and_get_mean(values));
    printf("Mean squared error: %lf\n",
        get_mean_squared_error(values, populate_values_and_get_mean(values)));
}
