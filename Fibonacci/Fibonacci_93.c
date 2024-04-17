#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_FIB_NUMBERS 1000000
#define BLOCK_SIZE 1000
#define NUM_THREADS (NUM_FIB_NUMBERS / BLOCK_SIZE)

pthread_mutex_t mutex;
long long fibonacci[NUM_FIB_NUMBERS];

void* calculate_fibonacci(void* arg) {
    int start = *(int*)arg;
    free(arg);

    for (int i = start; i < start + BLOCK_SIZE && i < NUM_FIB_NUMBERS; ++i) {
        if (i == 0) {
            fibonacci[i] = 0;
        } else if (i == 1) {
            fibonacci[i] = 1;
        } else {
            pthread_mutex_lock(&mutex);
            fibonacci[i] = fibonacci[i - 1] + fibonacci[i - 2];
            pthread_mutex_unlock(&mutex);
        }
    }

    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];
    pthread_mutex_init(&mutex, NULL);

    for (int i = 0; i < NUM_THREADS; ++i) {
        int *start = malloc(sizeof(int));
        *start = i * BLOCK_SIZE;
        if (pthread_create(&threads[i], NULL, calculate_fibonacci, (void*)start) != 0) {
            perror("Failed to create thread");
            return 1;
        }
    }

    for (int i = 0; i < NUM_THREADS; ++i) {
        pthread_join(threads[i], NULL);
    }

    FILE *file = fopen("fibonacci_93.txt", "w");
    if (file == NULL) {
        perror("Failed to open file");
        return 1;
    }

    for (int i = 0; i < NUM_FIB_NUMBERS; ++i) {
        fprintf(file, "Fib(%d) = %lld\n", i, fibonacci[i]);
    }

    fclose(file);
    pthread_mutex_destroy(&mutex);
    return 0;
}
