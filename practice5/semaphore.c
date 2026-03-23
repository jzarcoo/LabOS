#include "semaphore.h"
#include "scheduler.h"

extern tcb_t tasks[MAX_TASKS];
extern int current_task;

void k_sem_init(semaphore_t *sem, int valor_inicial) {
    sem->contador_recursos = valor_inicial;
    sem->head = 0;
    sem->tail = 0;
}

void k_sem_wait(semaphore_t *sem) {
    sem->contador_recursos--;

    if (sem->contador_recursos < 0) {
        tasks[current_task].state = BLOCKED;
        sem->tareas_esperando[sem->tail] = current_task;
        sem->tail = (sem->tail + 1) % MAX_TASKS;
        // Trigger PendSV to perform the context switch
        *(volatile uint32_t *)0xE000ED04 = (1 << 28);
    }
}


void k_sem_post(semaphore_t *sem) {
    sem->contador_recursos++;

    if (sem->contador_recursos <= 0) {
        int next_task = sem->tareas_esperando[sem->head];
        sem->head = (sem->head + 1) % MAX_TASKS;
        tasks[next_task].state = READY;
    }
}