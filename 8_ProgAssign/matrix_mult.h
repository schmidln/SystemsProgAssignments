/* Amaan Banga - bangaa@bc.edu
 * April 28, 2024
 * PA8 Header
 */

#include <stdbool.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/time.h>
#include <pthread.h>
#include <string.h>

#define DIM 1024
#define NUM_WORKERS 4
#define SUCCESS 0
#define FAILURE (-1)

typedef void (* multiply_function)(const double * const, const double * const, double * const, const int, const int);

typedef struct Args {
    const double * a;
    const double * b;
    double * c;
    int dim;
    int row_start;
    int num_workers;
} Args;

void multiply_parallel_threads(const double * const, const double * const, double * const, const int, const int);
void multiply_chunk(const double * const, const double * const, double * const, const int, const int, const int);
void init_matrix(double * , int);
void multiply_serial(const double * const, const double * const, double * const, const int, const int);
void print_matrix(double * , int);
void * mmap_checked(size_t);
void munmap_checked(void * , size_t);
pid_t fork_checked();
void multiply_parallel_processes(const double * const, const double * const, double * const, const int, const int);
void * task(void * );
int verify(const double * const, const double * const, const int);
struct timeval time_diff(struct timeval * , struct timeval * );
void print_elapsed_time(struct timeval * , struct timeval * , const char * const );
void print_verification(const double * const, const double * const, const int, const char * const);
void run_and_time(
        multiply_function multiply_matrices,
        const double * const a,
        const double * const b,
        double * const c,
        const double * const gold,
        const int dim,
        const char * const name,
        const int num_workers,
        const bool verify);




