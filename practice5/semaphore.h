#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include <stdint.h>
#include "scheduler.h"

typedef struct {
    int contador_recursos;
    int tareas_esperando[MAX_TASKS];
    int head;
    int tail;
} semaphore_t;

void k_sem_init(semaphore_t *sem, int valor_inicial);
void k_sem_wait(semaphore_t *sem);
void k_sem_post(semaphore_t *sem);

#endif 