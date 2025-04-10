/*
 * Authors: Andy Bello - belloac@bc.edu, Lucas Schmidt - schmidln@bc.edu
*/

#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>

#define MIN_DIM_POWER 3
#define MAX_DIM_POWER 10
#define MAX_VALUE 20
#define FALSE 0
#define TRUE 1
#define MICROSECONDS_PER_SECOND 1000000

void init(const int dim, int * const m) {
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            m[i * dim + j] = rand() % MAX_VALUE;
        }
    }
}

void multiply(const int dim, const int * const a, int * const b, int * const c){
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
	    c[i * dim + j] = 0;
	    for (int k = 0; k < dim; k++) {
	        c[i * dim + j] += a[i * dim + k] * b[k * dim + j];
	    }
	}
    } 
}

void transpose(const int dim, int * const m) {
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < i; j++) {
            int tmp = m[i * dim + j];
            m[i * dim + j] = m[j * dim + i];
            m[j * dim + i] = tmp;
        }
    }
}

void multiply_transpose(const int dim, const int * const a, int * const b_t, int * const c) {
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            c[i * dim + j] = 0;
            for (int k = 0; k < dim; k++) {
                c[i * dim + j] += a[i * dim + k] * b_t[j * dim + k];
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
) {
    struct timeval start;
    struct timeval end;
    struct timeval elapsed;
    gettimeofday(&start, NULL);
    mult_func(dim, a, b, c);
    gettimeofday(&end, NULL);
    elapsed.tv_sec = end.tv_sec - start.tv_sec;
    elapsed.tv_usec = end.tv_usec - start.tv_usec;
    if (elapsed.tv_usec < 0) {
        elapsed.tv_sec--;
        elapsed.tv_usec += MICROSECONDS_PER_SECOND;
    }
    return elapsed;
}

int verify(const int dim, const int * const c1, const int * const c2) {
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            if (c1[i * dim + j] != c2[i * dim + j]) {
                   return FALSE;
            }
        }
    }
    return TRUE;
}

double get_speedup(struct timeval * result1, struct timeval * result2) {
    double time1 = (double)result1->tv_sec + (double)result1->tv_usec / MICROSECONDS_PER_SECOND;
    double time2 = (double)result2->tv_sec + (double)result2->tv_usec / MICROSECONDS_PER_SECOND;
    return time1 / time2;
}

void run_test(const int dim) {
    int * a = calloc(dim * dim, sizeof(int));
    int * b = calloc(dim * dim, sizeof(int));
    int * c1 = calloc(dim * dim, sizeof(int));
    int * c2 = calloc(dim * dim, sizeof(int));
    init(dim, a);
    init(dim, b);
    init(dim, c1);
    init(dim, c2);
    printf("Testing on %d-by-%d square matrices.\n", dim, dim);
    struct timeval time_standard = run_and_time(&multiply, dim, a, b, c1);
    struct timeval time_with_transpose = run_and_time(&transpose_and_multiply, dim, a, b, c2);
    int v = verify(dim, c1, c2);
    if (v != 1) {
        printf("Multiplication algorithms do not agree!\n");
    }
    else {
        printf("Results agree.\n");
    }
    printf("Standard Multiplication: %ld seconds, %d microseconds\n", (long)time_standard.tv_sec, (int)time_standard.tv_usec);
    printf("Multiplication with transpose: %ld seconds, %d microseconds\n", (long)time_with_transpose.tv_sec, (int)time_with_transpose.tv_usec);
    double speedup = get_speedup(&time_standard, &time_with_transpose);
    printf("Speedup: %f\n", speedup);
    putchar('\n');
    free(a);
    free(b);
    free(c1);
    free(c2);
}

void print(const int dim, const int * const m) {
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            printf("   %d", m[i * dim + j]);
        }
        putchar('\n');
    }
}

int main() {
    for (int power = MIN_DIM_POWER; power <= MAX_DIM_POWER; power++) {
        run_test(1 << power);
    }
    return EXIT_SUCCESS;    
}
