#include <stdint.h>
#include "syscalls.h"

/* GPIO LED pin definition */
#define LED_PIN 25
/* GPIO Button pin definition */
#define BTN_PIN 15

/**
 * DELAY: Empty loop. We use 'volatile' to prevent the compiler 
 * from optimizing it away.
 * @param t: Number of iterations for the delay.
 */
void delay(int t){
  for (volatile int x = 0; x < t; x ++) {
	for (volatile int i = 0; i < 500000; i ++) ;
  }
}

int user_app_entry(void) {
    // Setup Phase (Unprivileged): The user program configures its resources using syscalls.
    sys_gpio_dir(LED_PIN, 1);
    sys_gpio_dir(BTN_PIN, 0);
    int led_state = 0;
    int prev_btn_state = 0;
    while (1) {
        int btn_state = sys_gpio_get(BTN_PIN);
        sys_gpio_set(LED_PIN, btn_state);
        // Falling edge detection for the button press
        if (btn_state == 1 && prev_btn_state == 0) {
            // Debounce
            delay(2);
            led_state = !led_state; 
            sys_gpio_set(LED_PIN, led_state);
        }
        prev_btn_state = btn_state;
    }
    return 0;
}