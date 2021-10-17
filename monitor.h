#ifndef MONITOR_H_
#define MONITOR_H_

#define NUMBER_OF_PHILOSOPHERS 5

// Inicializa a estrutura do monitor. Retorna 1 em caso de sucesso
// e 0 caso contrário.
unsigned short int monitor_initialize();

// A funcao "pegar palitos" é a responsável por controlar o recurso
// compartilhado. Utiliza-se de ferramentas para prevenção de deadlock.
// Retorna 1 em caso de sucesso e 0 caso contrário.
unsigned short int monitor_pickup(unsigned short int philosopher_number);

// A funcao "pegar palitos" é a responsável por liberar o acesso ao recurso
// compartilhado. Utiliza-se de ferramentas para prevenção de deadlock.
// Retorna 1 em caso de sucesso e 0 caso contrário.
unsigned short int monitor_putdown(unsigned short int philosopher_number);

// Finalizacao da estrutura do monitor.
// Retorna 1 em sucesso e 0 caso contrario.
unsigned short int monitor_destroy();


#endif // MONITOR_H_