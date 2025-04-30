/* Amaan Banga - bangaa@bc.edu
 * April 28, 2024
 * PA8
 */ 

#include "matrix_mult.h"

void init_matrix(double * matrix, int dim) {
    double k = 1.0;
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            matrix[i * dim + j] = k++;
        }
    }
}
void multiply_serial(const double * const a, const double * const b, double * const c, const int dim, const int num_workers) {
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            c[i * dim + j] = 0.0;
            for (int k = 0; k < dim; k++) {
                c[i * dim + j] += a[i * dim + k] * b[k * dim + j];
            }
        }
    }
}
void print_matrix(double * matrix, int dim) {
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            printf("%f ", matrix[i * dim + j]);
        }
        printf("\n");
    }
}
void * mmap_checked(size_t size) {
    void * ptr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (ptr == MAP_FAILED) {
        perror("Mapping failed");
        exit(EXIT_FAILURE);
    }
    return ptr;
}
void munmap_checked(void * map, size_t size) {
    if (munmap(map, size) == -1) {
        perror("Munmap has occured");
        exit(EXIT_FAILURE);
    }
}
pid_t fork_checked() {
    pid_t process_id = fork();
    if (process_id == -1) {
        perror("Fork Failed");
        exit(EXIT_FAILURE);
    }
    return process_id;
}
void multiply_chunk(const double * const a, const double * const b, double * const c, const int dim, const int num_workers, const int row_start) {
    int chunk_size = dim / num_workers;
    int row_end = (row_start + chunk_size >= dim) ? dim : row_start + chunk_size;
    for (int i = row_start; i < row_end; i++) {
        for (int j = 0; j < dim; j++) {
            c[i * dim + j] = 0.0;
            for (int k = 0; k < dim; k++) {
                c[i * dim + j] += a[i * dim + k] * b[k * dim + j];
            }
        }
    }
}
void multiply_parallel_processes(const double * const a, const double * const b, double * const c, const int dim, const int num_workers) {
    double * actual_c = (double *)mmap_checked(dim * dim * sizeof(double));
    int num_procs = num_workers - 1;
    int chunk_size = dim / num_workers;
    for (int i = 0; i < num_procs; i++) {
        pid_t process_id = fork_checked();
        if (process_id == 0) {
            multiply_chunk(a, b, actual_c, dim, num_workers, i * chunk_size);
            exit(EXIT_SUCCESS);
        }
    }
    multiply_chunk(a, b, actual_c, dim, num_workers, num_procs * chunk_size);
    int waiting;
    while (wait(&waiting) > 0);
    for (int i = 0; i < dim * dim; i++) {
        c[i] = actual_c[i];
    }
    munmap_checked(actual_c, dim * dim * sizeof(double));
}
void * task(void * arg) {
    Args * args = (Args *)arg;
    multiply_chunk(args->a, args->b, args->c, args->dim, args->num_workers, args->row_start);
    return NULL;
}
void multiply_parallel_threads(const double * const a, const double * const b, double * const c, const int dim, const int num_workers) {
    int num_threads = num_workers - 1;
    pthread_t threads[num_threads];
    Args arg_set[num_workers];
    int chunk = dim / num_workers;
    int row_start = 0;
    for (int i = 0; i < num_workers; ++i) {
        arg_set[i].a = a;
        arg_set[i].b = b;
        arg_set[i].c = c;
        arg_set[i].dim = dim;
        arg_set[i].num_workers = num_workers;
        arg_set[i].row_start = row_start;
        row_start += chunk;
    }
    int id = 0;
    for (; id < num_threads; ++id) {
        pthread_create(&threads[id], NULL, task, &arg_set[id]);
    }
    chunk = dim - arg_set[num_workers - 1].row_start;
    task(&arg_set[num_workers - 1]);
    for (int i = 0; i < num_threads; ++i) {
        pthread_join(threads[i], NULL);
    }
}
int verify(const double * const m1, const double * const m2, const int dim) {
    for (int i = 0; i < dim * dim; i++) {
        if (m1[i] != m2[i]) {
            return FAILURE;
        }
    }
    return SUCCESS;
}
struct timeval time_diff(struct timeval * start, struct timeval * end) {
    struct timeval diff;
    diff.tv_sec = end->tv_sec - start->tv_sec;
    diff.tv_usec = end->tv_usec - start->tv_usec;
    if (diff.tv_usec < 0) {
        diff.tv_sec--;
        diff.tv_usec += 1000000; 
    }
    return diff;
}
void print_elapsed_time(struct timeval * start, struct timeval * end, const char * const name) {
    struct timeval diff = time_diff(start, end);
    int num_workers = 0;
    if (strcmp(name, "serial") == 0) {
        num_workers = 1;
    } else {
        num_workers = NUM_WORKERS;
    }
    const char * worker_plural = (num_workers == 1) ? "worker" : "workers";
    const char * seconds_unit = (diff.tv_sec == 1) ? "second" : "seconds";
    const char * microseconds_unit = (diff.tv_usec == 1) ? "microsecond" : "microseconds";
    printf("Algorithm: %s with %d %s.\n", name, num_workers, worker_plural);
    printf("Time elapsed for %s: %ld %s and %ld %s.\n", name, (long)diff.tv_sec, seconds_unit, (long)diff.tv_usec, microseconds_unit);
}
void print_verification(const double * const m1, const double * const m2, const int dim, const char * const name) {
    int verification = verify(m1, m2, dim);
    const char * check =  (verification == 0) ? "success" : "failure";
    printf("Verification for %s: %s.\n", name, check);
}
void run_and_time(
        multiply_function multiply_matrices,
        const double * const a,
        const double * const b,
        double * const c,
        const double * const gold,
        const int dim,
        const char * const name,
        const int num_workers,
        const bool verify) {
    struct timeval start, end;
    gettimeofday(&start, NULL);
    multiply_matrices(a, b, c, dim, num_workers);
    gettimeofday(&end, NULL);
    print_elapsed_time(&start, &end, name);
    if (verify) {
        print_verification(c, gold, dim, name);
    }
}

