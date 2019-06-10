//
// Created by utnso on 27/05/19.
//

#include "metadata.h"

void init_METADATA () {
    LIST_METADATA = list_create();
}

metadata_tad* new_metadata_tad(describe_tad* describe) {
    metadata_tad* auxMetadata = malloc(sizeof(metadata_tad));
    auxMetadata->DESCRIBE = describe;
    auxMetadata->MEMORY_NUMBER = -1;
    return auxMetadata;
}

void free_metadata(metadata_tad* metadata) {
    free(metadata->DESCRIBE->nameTable);
    free(metadata->DESCRIBE->consistencia);
    free(metadata->DESCRIBE);
    free(metadata);
}

void update_metadata_memory_number (metadata_tad* metadata, uint32_t number) {
    metadata->MEMORY_NUMBER = number;
}

void load_METADATA(t_list* listDescribes) {
    if (list_size(LIST_METADATA) == 0) {
        void load_element(void* element){
            metadata_tad* metadata = new_metadata_tad(element);
            if ( string_equals_ignore_case(metadata->DESCRIBE->consistencia, CRITERIO_SC) ) {
                uint32_t memory_number = criterio_ramdom_memory_by(CRITERIO_SC);
                update_metadata_memory_number(metadata, memory_number);
            }
            list_add(LIST_METADATA, metadata);
        }

        list_iterate(listDescribes, load_element);
        // todo free
//        list_destroy(listDescribes);
    } else {
      // todo
    }
}

void print_metadata (t_log* log) {
    void print_element_stack(void* element){
        metadata_tad* metadata = element;
        describe_tad* describe = metadata->DESCRIBE;
        log_info(log,
                 "DESCRIBE => TABLA: <%s>\tCONSISTENCIA: <%s>\tPARTICIONES: <%d>\tCOMPACTACION: <%d>\tMEMORIA: <%d>",
                 describe->nameTable, describe->consistencia, describe->particiones, describe->compactacion, metadata->MEMORY_NUMBER);
    }

    list_iterate(LIST_METADATA, print_element_stack);
}

metadata_tad* search_table (char* table) {
    int _is_the_table(metadata_tad* m){
        return string_equals_ignore_case(m->DESCRIBE->nameTable, table);
    }

    metadata_tad* auxMetadata = list_find(LIST_METADATA, (void*) _is_the_table);

    return auxMetadata;
}

uint32_t get_memory_socket_from_metadata (char* table) {
    metadata_tad* auxMetadata = search_table(table);

    if (auxMetadata == NULL) {
        return -1;
    } else {
        memory_tad* memory = search_memory(auxMetadata->MEMORY_NUMBER);
        return memory->SOCKET;
    }
}