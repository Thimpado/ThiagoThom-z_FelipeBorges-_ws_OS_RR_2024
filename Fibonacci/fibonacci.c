#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <gmp.h>

#define NUM_FIB_NUMBERS 1000000
#define BLOCK_SIZE 1000
#define NUM_THREADS (NUM_FIB_NUMBERS / BLOCK_SIZE)

pthread_mutex_t mutex;
mpz_t fibonacci[NUM_FIB_NUMBERS];


// extrai o valor inteiro do argumento, armazena na variável 'start' e depois libera a memória alocada para o argumento
void* calculate_fibonacci(void* arg) {
    int start = *(int*)arg;
    free(arg);

    // loop que calcula a sequência
    for (int i = start; i < NUM_FIB_NUMBERS; ++i) {
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

    // percorre todo os elementos do array e prepara pra receber os elementos usando a biblioteca GMP, podendo receber variáveis com valores absurdos
    for (int i = 0; i < NUM_FIB_NUMBERS; ++i) {
        mpz_init(fibonacci[i]);
    }

    // criação das threads para cada 1000 termos da sequência
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

    FILE *file = fopen("fibonacci.txt", "w");
    if (file == NULL) {
        perror("erro na criação do arquivo");
        return 1;
    }

    for (int i = 0; i < NUM_FIB_NUMBERS; ++i) {
        gmp_fprintf(file, "Fib(%d) = %Zd\n", i, fibonacci[i]);
    }

    fclose(file);

    // liberação da memória alocada para as variáveis de múltipla precisão
    for (int i = 0; i < NUM_FIB_NUMBERS; ++i) {
        mpz_clear(fibonacci[i]);
    }

    pthread_mutex_destroy(&mutex);
    return 0;
}
