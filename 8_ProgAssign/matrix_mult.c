void init_matrix(double * matrix, int dim) {
    double k = 1;
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            matrix[i * dim + j] = k;
            k++;
        }
    }
}


void print_matrix(double * matrix, int dim) {
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            printf("%f ", matrix[i * dim + j]);
        }
        putchar('\n');
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

void multiply_chunk(const double * const a, const double * const b, double * const c, const int dim, const int start, int chunk) {
    for (int i = start; i < start + chunk; i++) {
        for (int j = 0; j < dim; j++) {
            c[i * dim + j] = 0;
            for (int k = 0; k < dim; k++) {
                c[i * dim + j] += a[i * dim + k] * b[k * dim + j];
            }
        }
    }
}

void * mmap_checked(size_t size) {
    void * mem = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0);
    if (mem == MAP_FAILED) {
        perror("Demand-zero memory allocation failure");
        exit(EXIT_FAILURE);
    }
    return mem;
}

void munmap_checked(void * map, size_t size) {
    if (munmap(map, size) == -1) {
        perror("Munmap failure");
        exit(EXIT_FAILURE);
    }
}

pid_t fork_checked() {
    pid_t process_id = fork();
    if (process_id == -1) {
        perror("Fork failure");
        exit(EXIT_FAILURE);
    }
    return process_id;
}

void multiply_parallel_processes(const double * const a, const double * const b, double * const c, const int dim, const int num_workers) {
    int num_procs = num_workers - 1;
    int chunk_size = dim / num_workers;
    double * shared = (double *)mmap_checked(dim * dim * sizeof(double));
    for (int i = 0; i < num_procs; i++) {
        int start_row = i * chunk_size;
        pid_t pid = fork_checked();
        if (pid == 0) {
            multiply_chunk(a, b, shared, dim, start_row, chunk_size);
            exit(EXIT_SUCCESS);
        }
    }
    int start_row = num_procs * chunk_size;
    multiply_chunk(a, b, shared, dim, start_row, dim - start_row);
    int waiting;
    while (wait(&waiting) > 0);
    for (int i = 0; i < dim * dim; i++) {
        c[i] = shared[i];
    }
    munmap_checked(shared, dim * dim * sizeof(double));
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
    const char * microseconds_string = (diff.tv_usec == 1) ? "microsecond" : "microseconds";
    const char * seconds_string = (diff.tv_sec == 1) ? "second" : "seconds";
    const char * worker_string = (num_workers == 1) ? "worker" : "workers";
    printf("Algorithm: %s with %d %s.\n", name, num_workers, worker_string);
    printf("Time elapsed for %s: %ld %s and %ld %s.\n", name, (long)diff.tv_sec, seconds_string, (long)diff.tv_usec, microseconds_string);
}

void print_verification(const double * const m1, const double * const m2, const int dim, const char * const name) {
    int verification = verify(m1, m2, dim);
    const char * verify_result = (verification == 0) ? "success" : "failure";
    printf("Verification for %s: %s.\n", name, verify_result);
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
        const bool verify
        ) {
    struct timeval start, end;
    gettimeofday(&start, NULL);
    multiply_matrices(a, b, c, dim, num_workers);
    gettimeofday(&end, NULL);
    print_elapsed_time(&start, &end, name);
    if (verify) {
        print_verification(c, gold, dim, name);
    }
}

void * task(void * arg) {
    Args * args = (Args *)arg;
    multiply_chunk(args->a, args->b, args->c, args->dim, args->row_start, args->chunk);
    return NULL;
}

void multiply_parallel_threads(const double * const a, const double * const b, double * const c, const int dim, const int num_workers) {
    int num_threads = num_workers - 1;
    pthread_t threads[num_threads];
    int chunk = dim / num_workers;
    Args arg_set[num_workers];
    int row_start = 0;
    for (int i = 0; i < num_workers; ++i) {
        arg_set[i].a = a;
        arg_set[i].b = b;
        arg_set[i].c = c;
        arg_set[i].dim = dim;
        arg_set[i].row_start = row_start;
        arg_set[i].chunk = chunk;
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
