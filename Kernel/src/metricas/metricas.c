//
// Created by utnso on 28/05/19.
//

#include "metricas.h"

void init_metrics() {
    LIST_METRICS = list_create();
    TOTAL_INSERTS = 0;
    TOTAL_SELECTS = 0;
}

metric_tad* new_metric_value(uint32_t memoryNumber) {
    metric_tad* value = malloc(sizeof(metric_tad));
    value->MEMORY_NUMBER = memoryNumber;
    value->INSERTS = 0;
    value->SELECTS = 0;
    return value;
}

void free_metric_value(metric_tad* value) {
    free(value);
}

void metric_select(uint32_t memoryNumber) {
    TOTAL_SELECTS += 1;
    metric_tad* value = search_memory_metrics(memoryNumber);
    value->SELECTS += 1;
}

void metric_insert(uint32_t memoryNumber) {
    TOTAL_INSERTS += 1;
    metric_tad* value = search_memory_metrics(memoryNumber);
    value->INSERTS += 1;
}

void showMetrics (t_log* log_Kernel) {
    log_info(log_Kernel, "--------------------------------");
    log_info(log_Kernel, "Metricas");
    log_info(log_Kernel, "Read Latency");
    log_info(log_Kernel, "Write Latency");
    log_info(log_Kernel, "Read <%d>", TOTAL_SELECTS);
    log_info(log_Kernel, "Write <%d>", TOTAL_INSERTS);
    log_info(log_Kernel, "Memory Load");
    log_info(log_Kernel, "--------------------------------");
    if (list_size(LIST_METRICS) != 0) {
        list_destroy_and_destroy_elements(LIST_METRICS, free_metric_value);
        init_metrics();
    }
}

metric_tad* search_memory_metrics(uint32_t number) {
    int _is_the_memory(metric_tad* m){
        return m->MEMORY_NUMBER == number;
    }

    metric_tad* value = list_find(LIST_METRICS, (void*) _is_the_memory);

    if (value == NULL) {
        metric_tad* newValue = new_metric_value(number);
        list_add(LIST_METRICS, newValue);
        return newValue;
    } else {
        return value;
    }
}