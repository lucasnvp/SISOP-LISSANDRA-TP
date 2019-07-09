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
    free_describe_tad(metadata->DESCRIBE);
    free(metadata);
}

void update_metadata_memory_number (metadata_tad* metadata, uint32_t number) {
    metadata->MEMORY_NUMBER = number;
}

void load_METADATA(t_list* listDescribes) {
    if (list_size(LIST_METADATA) == 0) {
        create_and_add(listDescribes, LIST_METADATA);
    } else {
        t_list* LIST_METADATA_AUX = list_create();
        create_and_add(listDescribes, LIST_METADATA_AUX);

        void compare_element(void* element) {
            metadata_tad* metadata = element;
            if (string_equals_ignore_case(metadata->DESCRIBE->consistencia, CRITERIO_SC)) {
                set_previous_memory_number(metadata);
            }
        }

        list_iterate(LIST_METADATA_AUX, compare_element);

        list_destroy_and_destroy_elements(LIST_METADATA, free_metadata);
        LIST_METADATA = LIST_METADATA_AUX;
    }
}

void create_and_add(t_list* listDescribes, t_list* listToAdd) {
    void load_element(void* element) {
        describe_tad* dTad = element;
        describe_tad* describe = new_describe_tad(
                dTad->nameTable,
                dTad->consistencia,
                dTad->particiones,
                dTad->compactacion);
        metadata_tad* metadata = new_metadata_tad(describe);
        if ( string_equals_ignore_case(metadata->DESCRIBE->consistencia, CRITERIO_SC) ) {
            uint32_t memory_number = criterio_ramdom_memory_by(CRITERIO_SC);
            update_metadata_memory_number(metadata, memory_number);
        }
        list_add(listToAdd, metadata);
    }

    list_iterate(listDescribes, load_element);
}

void add_create_to_metadata(create_tad* create) {
    describe_tad* describe = new_describe_tad(
            create->nameTable,
            create->consistencia,
            create->particiones,
            create->compactacion);
    metadata_tad* metadata = new_metadata_tad(describe);
    if ( string_equals_ignore_case(metadata->DESCRIBE->consistencia, CRITERIO_SC) ) {
        uint32_t memory_number = criterio_ramdom_memory_by(CRITERIO_SC);
        update_metadata_memory_number(metadata, memory_number);
    }
    list_add(LIST_METADATA, metadata);
}

void set_previous_memory_number(metadata_tad* metadata) {
    void set_element(void* element) {
        metadata_tad* metadataElement = element;
        if (string_equals_ignore_case(metadataElement->DESCRIBE->nameTable, metadata->DESCRIBE->nameTable)) {
            metadata->MEMORY_NUMBER = metadataElement->MEMORY_NUMBER;
        }
    }
    list_iterate(LIST_METADATA, set_element);
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
    } else if (string_equals_ignore_case(auxMetadata->DESCRIBE->consistencia, CRITERIO_SHC)) {
        uint32_t memoryNumber = criterio_shc(table);
        memory_tad* memory = search_memory(memoryNumber);
        return memory->SOCKET;
    } else {
        memory_tad* memory = search_memory(auxMetadata->MEMORY_NUMBER);
        return memory->SOCKET;
    }
}