#include <stdint.h>

// === MAPA DE MEMORIA ( RP2040 Datasheet ) ===

// 1. SUBSYSTEM RESETS ( Base : 0 x4000C000 )
// Controla que perifericos estan despiertos o dormidos .
#define RESETS_BASE 0x4000C000
#define RESETS_RESET (*(volatile uint32_t *) (RESETS_BASE + 0x0))
#define RESETS_DONE (*(volatile uint32_t *) (RESETS_BASE + 0x8))

// 2. IO BANK 0 ( Base : 0 x40014000 )
// Controla la funcion logica de los pines ( GPIO , UART , etc ) .
#define IO_BANK0_BASE 0x40014000
// Offsets de los pines (16, 17, 18, 19) a utilizar
#define GPIO16_CTRL (*( volatile uint32_t *) (IO_BANK0_BASE + 0x084))
#define GPIO17_CTRL (*( volatile uint32_t *) (IO_BANK0_BASE + 0x08c))
#define GPIO18_CTRL (*( volatile uint32_t *) (IO_BANK0_BASE + 0x094))
#define GPIO19_CTRL (*( volatile uint32_t *) (IO_BANK0_BASE + 0x09c))

// 3. SINGLE - CYCLE IO ( SIO ) ( Base : 0 xD0000000 )
// Controla el estado alto / bajo de los pines de forma rapida .
#define SIO_BASE 0xD0000000
#define SIO_GPIO_OE (*(volatile uint32_t *) (SIO_BASE + 0x020)) // Output Enable
#define SIO_GPIO_OUT (*(volatile uint32_t *) (SIO_BASE + 0x010)) // Output Value

// Pines a utilizar
#define LED_PIN_16 16
#define LED_PIN_17 17
#define LED_PIN_18 18
#define LED_PIN_19 19

// 0xF = 1111 en binario para manipular
// pines consecutivos 16, 17, 18, 19
#define LED_MASK (0xF << LED_PIN_16)

// Número de cuenta
int nocta[]={3,2,1,1,0,2,5,0,2};
// Número de dígitos
int n = 9;

/**
 * RETARDO: Ciclo vacio. Usamos ’volatile’ para que el compilador
 * no elimine este bucle.
 *
 * @param t: Número de iteraciones del retardo.
 */
void pausa(int t){
  for (volatile int x = 0; x < t; x ++) {
	for (volatile int i = 0; i < 500000; i ++) ;
  }
}

int main(){
  // --- PASO 1: BOOTSTRAPPING ( Despertar Hardware ) ---
  // Debemos quitar el Reset a dos componentes :
  // Bit 5: IO_BANK0 ( Logica de pines )
  // Bit 8: PADS_BANK0 ( Electricidad de pines )
  // Escribimos 0 en esos bits para sacarlos del reset .
  RESETS_RESET &= ~((1 << 5) | (1 << 8));

  // Esperamos hasta que el hardware confirme que desperto
  while ((RESETS_DONE & ((1 << 5) | (1 << 8))) != ((1 << 5) | (1 << 8)));

  // --- PASO 2: CONFIGURACION ---
  // Seleccionamos la Funcion 5 (SIO - Software Control)
  // para el Pin 16, 17, 18, 19
  GPIO16_CTRL = 5;
  GPIO17_CTRL = 5;
  GPIO18_CTRL = 5;
  GPIO19_CTRL = 5;

  // Habilitamos el Pin 16, 17, 18, 19 como SALIDA en el SIO
  SIO_GPIO_OE |= (1 << LED_PIN_16);
  SIO_GPIO_OE |= (1 << LED_PIN_17);
  SIO_GPIO_OE |= (1 << LED_PIN_18);
  SIO_GPIO_OE |= (1 << LED_PIN_19);

  // --- PASO 3: BUCLE INFINITO ( Kernel Loop ) ---
  while (1) {
	for(int i=0; i<n;i++){
	  int d = nocta[i];
	  // Usamos la máscara para apagar los pines 16, 17, 18, 19
	  SIO_GPIO_OUT &= ~LED_MASK;
	  // Encender pines del número de cuenta
	  SIO_GPIO_OUT |= (d << LED_PIN_16);
	  // Retardo
	  pausa(20);
	  // Usamos la máscara para apagar los pines 16, 17, 18, 19
	  SIO_GPIO_OUT &= ~LED_MASK;
	  // Retardo
	  pausa(10);
	}
  }
  return 0;
}
