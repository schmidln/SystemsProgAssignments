/* Amaan Banga - bangaa@bc.edu
 * April 8, 2024
 * Systems PA5
 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#define MIN_DIM_POWER 3
#define MAX_DIM_POWER 10
#define MAX_VALUE 20
#define FALSE 0
#define TRUE 1

void init(const int dim, int * const m) {
     for (int i = 0; i < dim * dim; ++i) {
        m[i] = rand() % MAX_VALUE;
    }
}

void multiply(const int dim, const int * const a, int * const b, int * const c) {
    for (int i = 0; i < dim; ++i) {
        for (int j = 0; j < dim; ++j) {
            c[i * dim + j] = 0;
            for (int k = 0; k < dim; ++k) {
                c[i * dim + j] += a[i * dim + k] * b[k * dim + j];
            }
        }
    }    
}

void transpose(const int dim, int * const m) {
    for (int i = 0; i < dim; ++i) {
        for (int j = 0; j < i; ++j) {
            int temp = m[i * dim + j];
            m[i * dim + j] = m[j * dim + i];
            m[j * dim + i] = temp;
        }
    }
}

void multiply_transpose(const int dim, const int * const a, const int * const b_t, int * const c) {
    for (int i = 0; i < dim; ++i) {
        for (int j = 0; j < dim; ++j) {
            c[i * dim + j] = 0;
            const int * row_a = &a[i * dim];
            const int * row_b_t = &b_t[j * dim];
            for (int k = 0; k < dim; ++k) {
                c[i * dim + j] += row_a[k] * row_b_t[k];
            }
        }
    }
}

void transpose_and_multiply(const int dim, const int * const a, int * const b, int * const c) {
    transpose(dim, b);
    multiply_transpose(dim, a, b, c);
}

struct timeval run_and_time(
        void (* mult_func)(const int, const int * const, int * const, int * const),
        const int dim,
        const int * const a,
        int * const b,
        int * const c
        ){
    struct timeval start, end;
    gettimeofday(&start, NULL);
    mult_func(dim, a, b, c);
    gettimeofday(&end, NULL);
    struct timeval elapsed_time;
    elapsed_time.tv_sec = end.tv_sec - start.tv_sec;
    elapsed_time.tv_usec = end.tv_usec - start.tv_usec;
    if (elapsed_time.tv_usec < 0) {
        elapsed_time.tv_sec--;
        elapsed_time.tv_usec += 1000000;
    }

    return elapsed_time;    
} 

int verify(const int dim, const int * const c1, const int * const c2) {
    for (int i = 0; i < dim; ++i) {
        for (int j = 0; j < dim; ++j) {
            if (c1[i * dim + j] == c2[i * dim + j])
                return TRUE;
        }
    }
    return FALSE;
}

void run_test(const int dim) {
    int * a = calloc(dim * dim, sizeof(int));
    int * b = calloc(dim * dim, sizeof(int));
    int * b_t = calloc(dim * dim, sizeof(int));
    int * c1 = calloc(dim * dim, sizeof(int));
    int * c2 = calloc(dim * dim, sizeof(int));
    init(dim, a);
    init(dim, b);
    struct timeval elapsed_time_multiply = run_and_time(multiply, dim, a, b, c1);
    struct timeval elapsed_time_transpose = run_and_time(transpose_and_multiply, dim, a, b, c2); 
    int check = verify(dim, c1, c2);
    if (check == 1) {
        printf("Results agree.\n");
    } else {
        printf("Results Disagree. \n");
    }
    printf("Standard multiplication: %ld seconds, %d microseconds\n", elapsed_time_multiply.tv_sec, elapsed_time_multiply.tv_usec);
    printf("Multiplication with transpose: %ld seconds, %d microseconds\n", elapsed_time_transpose.tv_sec, elapsed_time_transpose.tv_usec);
    free(a);
    free(b);
    free(b_t);
    free(c1);
    free(c2);
}

void print(const int dim, const int * const m) {
     for (int i = 0; i < dim; ++i) {
        for (int j = 0; j < dim; ++j) {
            printf("%d ", m[i * dim + j]);
        }
        printf("\n");
    }
}

int main() {
    for (int power = MIN_DIM_POWER; power <= MAX_DIM_POWER; ++power) { 
    run_test(1 << power);
    }
    return EXIT_SUCCESS; 

}

