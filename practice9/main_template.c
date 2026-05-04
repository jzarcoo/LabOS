#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"

// TODO 1: Si el mecanismo que justificaste en tu análisis requiere
// interactuar
// con otros subsistemas del hardware (como Interrupciones o DMA),
// incluye sus
// librerías aquí arriba (ej. "hardware/irq.h" o "hardware/dma.h").

// Definicion de parametros del hardware
#define UART_ID uart0
#define BAUD_RATE 115200
#define UART_TX_PIN 0
#define UART_RX_PIN 1

// Requerimiento del caso de estudio: Exactamente 16 bytes
#define KEY_LENGTH 16

// Buffer global para guardar la clave.
char challenge_key[KEY_LENGTH + 1];

// Variable de control (Util si tu implementacion es asincrona)
volatile bool key_received = false;

// =============================================================
// TODO 2: (Opcional) Zona de Rutinas de Servicio de Interrupcion (ISR)
// Si justificaste que el sistema debe usar Interrupciones o DMA,
// probablemente
// necesites definir tu handler de interrupcion aqui. Si elegiste
// Polling,
// puedes dejar esta zona en blanco.
// =============================================================

/*
void on_uart_rx() {
    // Tu logica de ISR aqui
}
*/

void secure_boot_init() {
    // Inicializa la salida estandar (para ver los printf en consola)
    stdio_init_all();

    // =============================================================
    // TODO 3: Inicializacion Basica del UART
    // Configura el UART con el ID y Baud Rate definidos arriba.
    // Mapea los pines TX y RX a su funcion de hardware correspondiente
    // usando
    // gpio_set_function(PIN, GPIO_FUNC_UART);
    // =============================================================

    // =============================================================
    // TODO 4: Configuracion Especifica del Mecanismo (Polling / IRQ / DMA)
    // Dependiendo de tu analisis del correo del gerente, configura el
    // hardware:
    //   - ¿Necesitas que las FIFOs de hardware esten encendidas o apagadas?
    //   - ¿Debes habilitar o bloquear las interrupciones en el NVIC del
    //     procesador?
    //   - ¿Necesitas configurar un canal de Acceso Directo a Memoria?
    //   Hazlo aqui:
    // =============================================================

}

int main() {
    // Llamamos a la configuracion inicial
    secure_boot_init();

    // Damos un segundo para que la terminal serial conecte antes de imprimir
    sleep_ms(1000);

    printf("\n== MODULO DE ARRANQUE SEGURO ==\n");
    printf("Sistema en modo bloqueo. Esperando clave de %d bytes...\n", KEY_LENGTH);
    printf("Ingresa los caracteres en la terminal:\n> ");

    // =============================================================
    // TODO 5: Logica Principal de Recepcion
    // Aqui ocurre la magia. Dependiendo de tu eleccion arquitectonica:
    //
    // - Si elegiste Polling: Implementa aqui tu bucle de espera activa leyendo
    //   directamente los registros del UART hasta llenar `challenge_key`
    //
    // - Si elegiste Interrupciones o DMA: El trabajo duro ocurre en el hardware o
    //   en la ISR. ¿Que debe estar haciendo el procesador en esta seccion del main
    //   mientras espera que la variable `key_received` cambie a true?
    // =============================================================

    // --- TU CODIGO INICIA AQUI ---


    // --- TU CODIGO TERMINA AQUI ---

    // Aseguramos que el arreglo sea un string valido en C para poder imprimirlo
    challenge_key[KEY_LENGTH] = '\0';
    // Si el flujo del programa llega a esta linea, la recepcion termino.
    printf("\n\n[EXITO] Clave recibida: %s\n", challenge_key);
    printf("[SISTEMA] Validando integridad...\n");
    printf("[SISTEMA] Levantando RTOS...\n");

    // Bucle infinito simulando el ciclo de vida normal del sistema operativo
    while (1) {
        tight_loop_contents();
    }

    return 0;
}