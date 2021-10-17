#include "monitor.h"
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

// Definindo os dados para criação da estrutura do monitor (não está no
// monitor.h porque em um monitor, a estrutura é a mais abtrata o possível).
// O programa principal não sabe como o acesso ao recurso é feito.
struct monitor_DataType
{
    // Declaracao da variavel condicional para evitarmos starvation
    pthread_cond_t conditional_variables[NUMBER_OF_PHILOSOPHERS];
    enum {THINKING, HUNGRY, EATING} state[NUMBER_OF_PHILOSOPHERS];

    // Mutex para controlar o acesso aos palitos
    pthread_mutex_t lock;
};

struct monitor_DataType monitor_data;

// Funcao "privada" usada na construcao do TAD
void __test(int philosopher_number);

// Inicializa a estrutura do monitor. Retorna 1 em caso de sucesso
// e 0 caso contrário.
unsigned short int monitor_initialize()
{
    for (int i = 0; i < NUMBER_OF_PHILOSOPHERS; i++)
    {
        monitor_data.state[i] = THINKING;
        pthread_cond_init( &monitor_data.conditional_variables[i], NULL );
    }
    pthread_mutex_init( &monitor_data.lock, NULL );

    return 1;
}

// A funcao "pegar palitos" é a responsável por controlar o recurso
// compartilhado. Utiliza-se de ferramentas para prevenção de deadlock.
// Retorna 1 em caso de sucesso e 0 caso contrário.
unsigned short int monitor_pickup(unsigned short int philosopher_number)
{
    pthread_mutex_lock(&monitor_data.lock);

    monitor_data.state[philosopher_number] = HUNGRY;
    printf("Philosopher %d: hungry.\n", philosopher_number);
    __test(philosopher_number);

    while (monitor_data.state[philosopher_number] != EATING)
    {
        pthread_cond_wait(&monitor_data.conditional_variables[philosopher_number], &monitor_data.lock);
    }

    pthread_mutex_unlock(&monitor_data.lock);
    return 1;
}

// A funcao "pegar palitos" é a responsável por liberar o acesso ao recurso
// compartilhado. Utiliza-se de ferramentas para prevenção de deadlock.
// Retorna 1 em caso de sucesso e 0 caso contrário.
unsigned short int monitor_putdown(unsigned short int philosopher_number)
{
    pthread_mutex_lock(&monitor_data.lock);

    monitor_data.state[philosopher_number] = THINKING;

    __test((philosopher_number + (NUMBER_OF_PHILOSOPHERS - 1) ) % NUMBER_OF_PHILOSOPHERS);
    __test((philosopher_number + 1) % NUMBER_OF_PHILOSOPHERS);

    pthread_mutex_unlock(&monitor_data.lock);
    return 1;
}

// A funcao test é a responsável por dar o signal na thread que está esperando
// para ter o acesso ao recurso. No nosso problema, verificamos se os filosofos
// ao lado estão comendo (palitos vizinhos ocupados), e caso não, determinado
// filosofo começa a comer, ocupando os palitos e impedindo que a próxima função
// test executada no vizinho não faça nada. É impossível acontecer escrita
// ou leitura no vetor state durante a execução de test porque sempre travamos
// o mutex antes de chamá-la nas funções superiores (assim como nos outros momentos
// em que escrevemos o estado de HUNGRY OU THINKING)
void __test(int philosopher_number)
{
    if  ((monitor_data.state[(philosopher_number + (NUMBER_OF_PHILOSOPHERS - 1) ) % NUMBER_OF_PHILOSOPHERS] != EATING) &&
        (monitor_data.state[philosopher_number] == HUNGRY) &&
        (monitor_data.state[(philosopher_number + 1) % NUMBER_OF_PHILOSOPHERS] != EATING))
    {
        monitor_data.state[philosopher_number] = EATING;
        pthread_cond_signal( &monitor_data.conditional_variables[philosopher_number]);
    }
}

// Finalizacao da estrutura do monitor.
// Retorna 1 em sucesso e 0 caso contrario.
unsigned short int monitor_destroy()
{
    // Todos os dados da estrutura de dados monitor sao feitos na stack,
    // portanto, nao ha nenhuma liberacao de memoria a ser feita.
    return 1;
}