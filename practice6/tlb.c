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
    for (int i = 0; i < TLB_SIZE; i++) {
        if (tlb[i].valid && tlb[i].page == page) {
            tlb[i].last_access = global_timer; 
            printf("TLB HIT: Page %d found in TLB (Frame %d)\n", page, tlb[i].frame);
            return tlb[i].frame;
        }
    }
    // Si no se encuentra (MISS), notificar y retornar -1
    printf("TLB MISS: Page %d not in TLB\n", page);
    return -1;
}

void tlb_insert(int page, int frame) {
    global_timer++;
    
    // TODO: Alumno - Si el TLB no esta lleno, insertar en una entrada vacia
    int entry_index = -1;
    for (int i = 0; i < TLB_SIZE; i++) {
        if (tlb[i].valid == 0) {
            entry_index = i;
            break;
        }
    }

    // TODO: Alumno - Si esta lleno, usar algoritmo LRU para desalojar una entrada
    // TODO: Alumno - En caso de empate en LRU, usar FIFO (usar insertion_time)
    if (entry_index == -1) {

        int lru_index = 0;

        for (int i = 1; i < TLB_SIZE; i++) {
            if (tlb[i].last_access < tlb[lru_index].last_access) {
                lru_index = i;
            } else if (tlb[i].last_access == tlb[lru_index].last_access) {
                if (tlb[i].insertion_time < tlb[lru_index].insertion_time) {
                    lru_index = i;
                }
            }
        }
        entry_index = lru_index;

        printf("TLB EVICT: Entry %d (Page %d) discarded for Page %d\n",
            lru_index, tlb[lru_index].page, page);
    }

    // Insertar la nueva entrada en el TLB
    tlb[entry_index].valid = 1;
    tlb[entry_index].page = page;
    tlb[entry_index].frame = frame;
    tlb[entry_index].last_access = global_timer;
    tlb[entry_index].insertion_time = global_timer; 
    global_timer++;
    
    printf("TLB INSERT: Page %d -> Frame %d (Entry %d)\n", page, frame, entry_index);
}
