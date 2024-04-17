#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <gmp.h>

#define NUM_FIB_NUMBERS 1000000
#define BLOCK_SIZE 1000
#define NUM_THREADS (NUM_FIB_NUMBERS / BLOCK_SIZE)

pthread_mutex_t mutex;
mpz_t fibonacci[NUM_FIB_NUMBERS];

void* calculate_fibonacci(void* arg) {
    int start = *(int*)arg;
    free(arg);

    for (int i = start; i < start + BLOCK_SIZE && i < NUM_FIB_NUMBERS; ++i) {
        if (i == 0) {
            mpz_set_ui(fibonacci[i], 0);
        } else if (i == 1) {
            mpz_set_ui(fibonacci[i], 1);
        } else {
            pthread_mutex_lock(&mutex);
            mpz_add(fibonacci[i], fibonacci[i - 1], fibonacci[i - 2]);
            pthread_mutex_unlock(&mutex);
        }
    }

    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];
    pthread_mutex_init(&mutex, NULL);

    for (int i = 0; i < NUM_FIB_NUMBERS; ++i) {
        mpz_init(fibonacci[i]);
    }

    for (int i = 0; i < NUM_THREADS; ++i) {
        int *start = malloc(sizeof(int));
        *start = i * BLOCK_SIZE;
        if (pthread_create(&threads[i], NULL, calculate_fibonacci, (void*)start) != 0) {
            perror("erro na criação da thread");
            return 1;
        }
    }

    for (int i = 0; i < NUM_THREADS; ++i) {
        pthread_join(threads[i], NULL);
    }

    FILE *file = fopen("fibonacci_1000.txt", "w");
    if (file == NULL) {
        perror("erro na criação do arquivo");
        return 1;
    }

    for (int i = 0; i < NUM_FIB_NUMBERS; ++i) {
        gmp_fprintf(file, "Fib(%d) = %Zd\n", i, fibonacci[i]);
    }

    fclose(file);

    // Limpar os objetos mpz_t
    for (int i = 0; i < NUM_FIB_NUMBERS; ++i) {
        mpz_clear(fibonacci[i]);
    }

    pthread_mutex_destroy(&mutex);
    return 0;
}
