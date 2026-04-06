#include <stdio.h>
#include "tlb.h"

tlb_entry_t tlb[TLB_SIZE];
int global_timer = 0; // Simulated clock for LRU/FIFO

void init_tlb() {
    for (int i = 0; i < TLB_SIZE; i++) {
        tlb[i].valid = 0;
        tlb[i].page = -1;
        tlb[i].frame = -1;
        tlb[i].last_access = 0;
        tlb[i].insertion_time = 0;
    }
}

int tlb_lookup(int page) {
    global_timer++;
    // TODO: Alumno - Buscar la pagina en el TLB
    // TODO: Alumno - Si se encuentra (HIT), actualizar last_access y retornar el frame
    // Si no se encuentra (MISS), notificar y retornar -1
    printf("TLB MISS: Page %d not in TLB\n", page);
    return -1;
}

void tlb_insert(int page, int frame) {
    global_timer++;
    
    // TODO: Alumno - Si el TLB no esta lleno, insertar en una entrada vacia
    // TODO: Alumno - Si esta lleno, usar algoritmo LRU para desalojar una entrada
    // TODO: Alumno - En caso de empate en LRU, usar FIFO (usar insertion_time)
    
    printf("TLB INSERT: Page %d -> Frame %d (Entry %d)\n", page, frame, 0 /* entry_index */);
}
