#include <stdio.h>
#include <stdint.h>
#include "syscalls.h"
#include "semaphore.h"

/* Pin Definitions */
#define LED_0 16
#define LED_1 17
#define LED_2 18
#define LED_3 19
#define LED_4 20

static int sem_initialized = 0;
semaphore_t sem_radio;
semaphore_t sem_energy;

/**
 * @brief Busy-wait for an exact number of core cycles.
 * @param cycles Number of cycles to delay.
 */
static inline void delay_cycles_exact(uint32_t cycles)
{
    __asm volatile(
        "1: \n"
        "sub %[c], %[c], #1 \n" // 1 cycle
        "bne 1b \n"             // 1-3 cycles (depends on branch)
        : [c] "+r"(cycles)
        :
        : "cc");
}

/**
 * @brief Configure LED GPIOs as outputs.
 */
void setup_leds(void)
{
    sys_gpio_dir(LED_0, 1);
    sys_gpio_dir(LED_1, 1);
    sys_gpio_dir(LED_2, 1);
    sys_gpio_dir(LED_3, 1);
    sys_gpio_dir(LED_4, 1);
}

void setup_semaphores(void) {
    if (sem_initialized==1) return;
    k_sem_init(&sem_radio, 1); 
    k_sem_init(&sem_energy, 3); 
    sem_initialized = 1;
}

/**
 * @brief Binary counter task that updates LEDs.
 */
void task_counter(void)
{
    setup_leds();

    // This variable retains its value across context switches
    // because it lives on this task's stack.

    for (uint32_t count = 0; count < 32; count++)
    {
        sys_gpio_set(LED_0, (count >> 0) & 1);
        sys_gpio_set(LED_1, (count >> 1) & 1);
        sys_gpio_set(LED_2, (count >> 2) & 1);
        sys_gpio_set(LED_3, (count >> 3) & 1);
        sys_gpio_set(LED_4, (count >> 4) & 1);

        delay_cycles_exact(41700000 / 4);
    }
    
    // After counting to 31, the task can terminate itself.
    sys_exit();
}

/**
 * @brief Ping-pong LED animation task.
 */
void task_animation(void)
{
    setup_leds();

    // Persistent state variables on the stack
    int pins[] = {LED_0, LED_1, LED_2, LED_3, LED_4};
    int active_led = 0;
    int direction = 1;

    while (1)
    {
        // Turn off all
        for (int i = 0; i < 5; i++)
        {
            sys_gpio_set(pins[i], 0);
        }

        // Turn on current
        sys_gpio_set(pins[active_led], 1);

        // Compute next position
        active_led += direction;

        if (active_led >= 4)
        {
            direction = -1;
        }
        else if (active_led <= 0)
        {
            direction = 1;
        }

        delay_cycles_exact(41700000 / 20);
    }
}

/**
 * @brief Infinite task that rapidly blinks LED_1.
 */
void task3_infinite(void)
{
    // TODO: Implement an infinite task that rapidly blinks a different LED (e.g., LED_1).
}

/**
 * @brief Finite task that blinks an LED 5 times and then terminates.
 */
void task4_finite(void)
{
    // TODO: Implement a finite task that blinks an LED 5 times and then terminates.
}

void subsystem_task(int led, int id){
    sys_gpio_dir(led, 1);
    setup_semaphores();
    while(1){
        // thermal conditioning
        sys_sem_wait(&sem_energy);

        sys_gpio_set(led, 1);
        delay_cycles_exact(62500000); // 500 ms
        sys_gpio_set(led, 0);

        sys_sem_post(&sem_energy);

        // telemetry reporting
        sys_sem_wait(&sem_radio);

        printf("[SUBSISTEMA %d] Calentamiento finalizado. Operación nominal alcanzada. Transfiriendo paquetes de telemetría y reportando estado actual a base terrestre...\n", id);

        sys_sem_post(&sem_radio);
    }
}

void task1(void){
    subsystem_task(LED_0, 1);
}
void task2(void){
    subsystem_task(LED_1, 2);
}
void task3(void){
    subsystem_task(LED_2, 3);
}
void task4(void){
    subsystem_task(LED_3, 4);
}
void task5(void){
    subsystem_task(LED_4, 5);
}

/**
 * @brief User application entry point (unused).
 */
void user_app_entry(void)
{
    // Unused, tasks start directly
}