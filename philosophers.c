/*
    Problema a ser solucionado:

    Implemente baseado no modelo de monitor o problema do jantar dos filósofos,
    de maneira que não exista a possibilidade de deadlock e nem de starvation.
    Cada filósofo deve estar em uma thread e os recursos que usam deverão ser
    compartilhados (os palitos). Implementar no Linux usando C.

        Sobre o codigo:
    O código segue um padrão de nomenclatura de variáveis em ingles, com comentários
    em português. O arquivo "philosophers.c" contem a main e é o programa consumidor
    da estrutura abstrata de dados "monitor" (implementada no arquivo de mesmo nome).

    Para a execução do código, é necessário usar os seguintes comandos no terminal
    (desde que no diretório contendo os arquivos):
    $   make all
    $   make run

*/
#include <pthread.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "monitor.h"
#include <unistd.h>

int philosophers[NUMBER_OF_PHILOSOPHERS] = {0, 1, 2, 3, 4};

// Execucao em loop da acao de um filosofo. Visto que usaremos
// a estrutura de monitor, as funcoes no programa principal sao
// usadas de forma abstrata, sem conhecimento de detalhes de implementacao.
// Dessa forma, as unicas funcoes executadas pelo filosofo sao "pegar palito" e
// "soltar palito", que sao implementadas pelo monitor com as respectivas regras
// de acesso ao recurso compartilhado (palitos)
void *philosophize_at_dinner(void *philosopher)
{
    assert(philosopher != NULL);

    int *philosopher_number = philosopher;
    while (1)
    {
        unsigned short int success = monitor_pickup(*philosopher_number);
        if (!success)
        {
            printf("Error with %d° philosopher taking the clutery.\n", *philosopher_number);
            exit(1);
        }
        printf("Philosopher %d: eating.\n", *philosopher_number);
        sleep(4);

        success = monitor_putdown(*philosopher_number);
        if (!success)
        {
            printf("Error with %d° philosopher releasing the clutery.\n", *philosopher_number);
            exit(2);
        }
        printf("Philosopher %d: thinking.\n", *philosopher_number);
        sleep(2);

    }

    return 0;
}

int main()
{
    pthread_t philosopher_thread[NUMBER_OF_PHILOSOPHERS];

    // Inicializando a estrutura monitor
    unsigned short int success = monitor_initialize();
    if (!success)
    {
        return -1;
    }

    // Criando as threads para simular a acao de cada filosofo
    for (int i = 0; i < NUMBER_OF_PHILOSOPHERS; i++)
    {
        pthread_create(&philosopher_thread[i], NULL,
                       philosophize_at_dinner, &philosophers[i]);
    }


    for (int i = 0; i < NUMBER_OF_PHILOSOPHERS; i++)
    {
        pthread_join(philosopher_thread[i], NULL);
    }

    // Destruindo a estrutura monitor
    monitor_destroy();

    return 0;
}
