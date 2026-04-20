#include <stdio.h>
#include "vm_bl.h"

#define MAX_PROCESSES 10

process_t processes[MAX_PROCESSES];
int process_count = 0;

void assign_process(int pid, int base, int limit) {
    if (process_count < MAX_PROCESSES) {
        // TODO: Alumno - Registrar el proceso con su base y limite
        processes[process_count].pid = pid;
        processes[process_count].base = base;
        processes[process_count].limit = limit;
        process_count++;
    } else {
        fprintf(stderr, "Error: Maximum number of processes reached\n");
    }
}

int base_limit(int pid, int address) {
    // TODO: Alumno - Buscar el proceso por PID
    // TODO: Alumno - Verificar si la direccion esta dentro del rango [base, base + limite)
    for(int i=0; i<process_count; i++) {
        if(processes[i].pid == pid) {
            return address >= processes[i].base && address < processes[i].base + processes[i].limit;
        }
    }
    // Retornar 1 si es valido, 0 si hay violacion, -1 si el proceso no existe
    return -1; // Placeholder
}

void ram_write(int pid, int address, unsigned char value) {
    int result = base_limit(pid, address);
    if (result == 1) {
        // TODO: Alumno - Realizar la escritura en la RAM
        ram[address] = value;
        printf("WRITE: Process %d wrote %02X to 0x%03X\n", pid, value, address);
    } else if (result == 0) {
        printf("ERROR: Process %d access violation at 0x%03X (Base-Limit)\n", pid, address);
    } else {
        printf("ERROR: Process %d not found\n", pid);
    }
}

unsigned char ram_read(int pid, int address) {
    int result = base_limit(pid, address);
    if (result == 1) {
        // TODO: Alumno - Realizar la lectura de la RAM
        unsigned char value = 0; // Cambiar por valor real de RAM
        value = ram[address];
        printf("READ: Process %d read %02X from 0x%03X\n", pid, value, address);
        return value;
    } else if (result == 0) {
        printf("ERROR: Process %d access violation at 0x%03X (Base-Limit)\n", pid, address);
    } else {
        printf("ERROR: Process %d not found\n", pid);
    }
    return 0;
}
