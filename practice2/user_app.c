#include <stdint.h>
#include "syscalls.h"

/* GPIO LED pin definition */
#define LED_PIN 25
/* GPIO Button pin definition */
#define BTN_PIN 15

int user_app_entry(void) {
    // Setup Phase (Unprivileged): The user program configures its resources using syscalls.

    // TODO: Configure LED_PIN as output and BTN_PIN as input using sys_gpio_dir syscall

    while (1) {
        // Operational Phase (Unprivileged): The user program performs its main functionality.
        // TODO: Read the button state using sys_gpio_get 
        // set the LED state accordingly using sys_gpio_set
    }
    
    // Should never reach here
    return 0;
}