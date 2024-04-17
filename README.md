# Primeiro Seminário feito para a matéria de Sistemas Operacionais

Alunos: Felipe Rubens de Sousa Borges e Thiago Thomáz Santana do Nascimento

Professor: Herbert Oliveira Rocha

## Tema abordado

Fibonacci em Multithread

O projeto a ser desenvolvido, consiste em gerar a sequência de números da serie de Fibonacci até o
número 1.000.000. 
- Utilizando threads para cada faixa de 1000 valores, sendo criada uma thread e disparando o
processo para cada uma delas. 

A sequência de Fibonacci é composta por uma sucessão de números descrita pelo famoso matemático 
italiano Leonardo de Pisa (1170-1250), mais conhecido como Fibonacci, no final do século 12. 
O matemático percebeu uma regularidade matemática a partir de um problema criado por ele mesmo. 
Assim, a fórmula a seguir define a sequência: Fn = Fn - 1 + Fn – 2. 

## Resolução e criação do programa

### Compilação e execução

Para compilar e executar os programas ```fibonacci.c``` e ```fibonacci_1000``` é necessário instalar a biblioteca GMP, a GNU Multiple-Precision Library, que é uma biblioteca de código aberto para aritmética de precisão arbitrária, trabalhando sobre inteiros, racionais e números de ponto flutuante. Essa biblioteca permite a que variáveis tenham um tamanho de bytes variável.

No Linux, use esse comando para instalá-la: ```sudo apt-get install libgmp-dev```.
No MacOS, use esse comando para instalá-la: ```brew install gmp```.

### Comunicação das Threads

Neste programa, as threads se comunicam principalmente por meio do uso de uma variável compartilhada e de um mutex (mutex é uma abreviação de "mutual exclusion", que significa exclusão mútua).

A variável compartilhada é o array fibonacci[], que armazena os números da sequência de Fibonacci. Como todas as threads têm acesso a este array, elas podem atualizar seus valores simultaneamente. Isso é potencialmente problemático, pois duas ou mais threads podem tentar atualizar o mesmo elemento do array ao mesmo tempo, causando condições de corrida e resultados imprevisíveis.

Para evitar isso, o programa utiliza um mutex (pthread_mutex_t mutex) para garantir que apenas uma thread por vez possa acessar e modificar o array fibonacci[]. Isso é feito por meio das chamadas pthread_mutex_lock() e pthread_mutex_unlock(). Quando uma thread precisa acessar ou modificar o array, ela bloqueia o mutex antes de fazer isso, garantindo que outras threads não possam acessá-lo simultaneamente. Depois que a operação é concluída, a thread desbloqueia o mutex, permitindo que outras threads possam acessar o array.

Em resumo, as threads se comunicam por meio do array compartilhado fibonacci[], com o mutex garantindo que apenas uma thread possa acessá-lo por vez, evitando condições de corrida e garantindo consistência nos cálculos e atualizações.

### Possível região crítica

Cada thread está inicializando os elementos do array fibonacci[] usando a função mpz_init(). Como todas as threads têm acesso ao mesmo array, pode haver um problema se várias threads tentarem inicializar o mesmo elemento do array ao mesmo tempo.

No entanto, neste caso, não há necessidade de usar o mutex, porque a função mpz_init() não modifica nenhum dado compartilhado. Ela apenas inicializa uma variável mpz_t, que é específica para cada elemento do array. Portanto, não há risco de condições de corrida neste ponto do código.


