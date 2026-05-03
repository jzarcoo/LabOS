#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/flash.h"
#include "hardware/sync.h"

// Constantes del FS
#define FS_BASE_OFFSET (1024 * 1024)  // Offset de 1MB (deja 1MB para el programa)
#define FS_BASE_ADDRESS (XIP_BASE + FS_BASE_OFFSET)
#define MAX_FILES 16
#define MAGIC_NUMBER 0x4649534F // "FISO"

// 8 bytes (cabecera) + 16 * 24 bytes (entradas) = 392 bytes totales
// Necesitamos exactamente 2 páginas (512 bytes) para programarlo por completo
#define META_PROGRAM_SIZE 512

typedef enum {
    STATUS_DELETED = 0x00,
    STATUS_OCCUPIED = 0xAA,
    STATUS_FREE = 0xFF
} FileStatus;

typedef struct {
    char name[12];
    uint32_t offset;
    uint32_t size;
    uint8_t status;
    uint8_t padding[3];
} FileEntry;

typedef struct {
    uint32_t magic_number;
    uint32_t total_size;
    FileEntry entries[MAX_FILES];
} MetaData_Table;

// Puntero para leer la memoria Flash directamente mapeada (XIP - Execute In Place)
const MetaData_Table* fs_meta = (const MetaData_Table*)(FS_BASE_ADDRESS);

// --- MÓDULO 1: Inicialización y Superbloque ---

// fs_format() se entrega resuelta como ejemplo de uso de la API de Flash
void fs_format() {
    // 1. Deshabilitar interrupciones antes de tocar la Flash
    uint32_t ints = save_and_disable_interrupts();

    // 2. Borrar el sector completo (4096 bytes)
    flash_range_erase(FS_BASE_OFFSET, FLASH_SECTOR_SIZE);

    // 3. Crear los nuevos metadatos en RAM
    MetaData_Table new_meta;
    memset(&new_meta, 0xFF, sizeof(MetaData_Table));
    new_meta.magic_number = MAGIC_NUMBER;
    new_meta.total_size = 0;

    // 4. Preparar el buffer del tamaño exacto (múltiplo de 256 bytes)
    uint8_t meta_buf[META_PROGRAM_SIZE];
    memset(meta_buf, 0xFF, META_PROGRAM_SIZE);
    memcpy(meta_buf, &new_meta, sizeof(MetaData_Table));

    // 5. Programar la Flash y restaurar interrupciones
    flash_range_program(FS_BASE_OFFSET, meta_buf, META_PROGRAM_SIZE);
    restore_interrupts(ints);

    printf("FS: Formateado exitosamente.\n");
}

void fs_init() {
    if (fs_meta->magic_number != MAGIC_NUMBER) {
        printf("FS Warning: Magic Number no encontrado. Forzando formateo...\n");
        fs_format();
        return;
    } 
    printf("FS Inicializado exitosamente. Magic Number: 0x%08X\n", fs_meta->magic_number);
}

// --- MÓDULO 2: Asignación de Metadatos y Alineación ---

int fs_create(const char* name) {
    if (strlen(name) >= 12) {
        printf("FS Error: Nombre muy largo.\n");
        return -1;
    }

    // TODO: CHALLENGE 2 - Creación de archivo
    // 1. Itera sobre fs_meta->entries buscando la primera entrada con status == STATUS_FREE.
    // 2. Durante la iteración, verifica que no exista ya un archivo con el mismo nombre y status == STATUS_OCCUPIED.
    //    Si existe, retorna error (-2).
    // 3. Calcula la variable 'next_free_offset'. Para un FS contiguo, es el offset del último archivo
    //    redondeado hacia arriba al siguiente FLASH_SECTOR_SIZE (4096).
    // 4. Copia los metadatos actuales a una variable local: MetaData_Table updated_meta;
    // 5. En 'updated_meta', asigna a la entrada libre encontrada: el nombre, el offset calculado,
    //    tamaño 0, y cambia su estado a STATUS_OCCUPIED.
    // 6. Escribe 'updated_meta' en la Flash (revisa fs_format para ver cómo borrar y programar).
    // 7. Retorna el índice (free_index) creado.

    return -1; // Elimina este return al implementar tu código
}

// --- MÓDULO 3: Read-Modify-Write ---

int fs_write(const char* name, const uint8_t* data, uint32_t size) {
    int target_idx = -1;

    // Búsqueda del archivo
    for (int i = 0; i < MAX_FILES; i++) {
        if (fs_meta->entries[i].status == STATUS_OCCUPIED &&
            strncmp(fs_meta->entries[i].name, name, 12) == 0) {
            target_idx = i;
            break;
        }
    }

    if (target_idx == -1) {
        printf("FS Error: Archivo '%s' no encontrado.\n", name);
        return -1;
    }

    // TODO: CHALLENGE 3A - Escritura Flash y Actualización de Metadatos
    // 1. Recupera el offset asignado para este archivo desde los metadatos.
    // 2. Calcula cuántos sectores requiere 'size' bytes. Recuerda que solo puedes borrar en
    //    múltiplos de FLASH_SECTOR_SIZE (4096).
    // 3. Usa save_and_disable_interrupts() y borra los sectores necesarios.
    // 4. Programa los datos en la Flash iterando en bloques de FLASH_PAGE_SIZE (256 bytes).
    //    Asegúrate de no escribir fuera del tamaño de los datos pasados en el parámetro.
    // 5. Restaura las interrupciones.
    // 6. Si el archivo cambió de tamaño, actualiza 'size' en una copia de los metadatos y reescribe
    //    el bloque del superbloque (Sector 0) para guardar el nuevo tamaño de forma persistente.

    return 0;
}

int fs_read(const char* name, uint8_t* buffer, uint32_t max_size) {
    // TODO: CHALLENGE 3B - Lectura XIP (Execute-In-Place)
    // 1. Busca el índice del archivo por su nombre (igual que en fs_write).
    // 2. Si no se encuentra, retorna -1.
    // 3. Obtén el 'file_size' real del archivo desde los metadatos.
    // 4. Determina cuánto vas a leer: el mínimo entre 'file_size' y 'max_size'.
    // 5. Calcula la dirección de memoria física: (XIP_BASE + offset).
    // 6. Usa memcpy() para copiar los bytes directamente de la dirección física al buffer.
    // 7. Retorna la cantidad de bytes leídos.

    return 0; // Cambia este return al finalizar
}
    
// --- MÓDULO 4: Borrado Lógico & Visualización ---

int fs_delete(const char* name) {
    // TODO: CHALLENGE 4A - Borrado lógico
    // 1. Busca el índice del archivo solicitado. Si no existe, retorna -1.
    // 2. Haz una copia de fs_meta en una variable local 'updated_meta'.
    // 3. Cambia el estado de la entrada encontrada a STATUS_DELETED.
    // 4. Usa las rutinas de Flash (erase y program) para sobrescribir los metadatos.
    // Nota: NO borres los datos del archivo en la Flash, solo actualiza el estado.

    return 0;
}

void fs_dump() {
    printf("\n=== Mapa de PicoFS ===\n");
    printf("Magic Number: 0x%08X\n", fs_meta->magic_number);
    printf("%-12s  %-10s | %-8s | %-8s\n", "Name", "Offset", "Size", "Status");
    printf("------------------------------------------------\n");

    // TODO: CHALLENGE 4B - Volcado de la tabla
    // 1. Itera sobre las entradas de metadatos (MAX_FILES).
    // 2. Si el estado es distinto a STATUS_FREE, imprime sus campos con formato similar al encabezado.
    // 3. Para el estado, convierte los códigos Hex a strings (Ej. "OCCUPIED" o "DELETED").

    printf("==============================\n\n");
}

// --- DEMOSTRACIÓN PRINCIPAL (No modificar el main) ---

int main() {
    stdio_init_all();
    sleep_ms(2000); // Esperar a que conecte el Monitor Serie USB

    printf("\n--- Iniciando Demostración de PicoFS ---\n");

    // Demo Módulo 1
    printf("\n[Módulo 1]: Inicialización\n");
    fs_init();

    printf("Formateando para iniciar la demo limpia...\n");
    fs_format();
    fs_init(); // Re-inicializar para mostrar que ahora sí detecta el Magic Number

    // Demo Módulo 2
    printf("\n[Módulo 2]: Asignación y Alineación\n");
    fs_create("file1.txt");
    fs_create("file2.bin");
    fs_create("image.png");
    fs_dump();

    // Demo Módulo 3
    printf("\n[Módulo 3]: Read-Modify-Write\n");
    const char* str1 = "Hola Mundo PicoFS!";
    fs_write("file1.txt", (const uint8_t*)str1, strlen(str1) + 1);

    // Escritura grande para comprobar el cálculo de sectores contiguos
    uint8_t dummy_bin[5000];
    memset(dummy_bin, 0x42, sizeof(dummy_bin));
    fs_write("file2.bin", dummy_bin, sizeof(dummy_bin)); // Requiere 2 sectores

    fs_dump();

    printf("\n[Test de lectura fs_read]:\n");
    char read_buf[64];
    fs_read("file1.txt", (uint8_t*)read_buf, sizeof(read_buf));
    printf("Contenido leído de 'file1.txt': %s\n", read_buf);

    // Demo Módulo 4
    printf("\n[Módulo 4]: Borrado Lógico\n");
    fs_delete("file1.txt");

    fs_dump();

    printf("\n--- Demostración Finalizada ---\n");

    while (1) {
        tight_loop_contents();
    }

    return 0;
}