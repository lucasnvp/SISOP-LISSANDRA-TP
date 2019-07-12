//
// Created by utnso on 28/05/19.
//

#include "metricas.h"

void init_metrics() {
    LIST_METRICS = list_create();
    LIST_READ_LATENCY_METRICS = list_create();
    LIST_WRITE_LATENCY_METRICS = list_create();
    TOTAL_INSERTS = 0;
    TOTAL_SELECTS = 0;
    READ_LATENCY = 0;
    WRITE_LATENCY = 0;
}

metric_tad* new_metric_value(uint32_t memoryNumber) {
    metric_tad* value = malloc(sizeof(metric_tad));
    value->MEMORY_NUMBER = memoryNumber;
    value->INSERTS = 0;
    value->SELECTS = 0;
    return value;
}

time_metric_tad* new_time_metric_value() {
    time_metric_tad* value = malloc(sizeof(time_metric_tad));
    time(&value->start);
    return value;
}

void free_metric_value(metric_tad* value) {
    free(value);
}

void free_time_metric_value(time_metric_tad* value) {
    free(value);
}

void clean_metrics() {
    if (list_size(LIST_METRICS) != 0) {
        list_destroy_and_destroy_elements(LIST_METRICS, free_metric_value);
        list_destroy(LIST_READ_LATENCY_METRICS);
        list_destroy(LIST_WRITE_LATENCY_METRICS);
//        list_destroy_and_destroy_elements(LIST_READ_LATENCY_METRICS, free_time_metric_value);
//        list_destroy_and_destroy_elements(LIST_WRITE_LATENCY_METRICS, free_time_metric_value);
        init_metrics();
    }
}

void metric_select(uint32_t memoryNumber) {
    pthread_mutex_lock(&mutexMetrics);
    TOTAL_SELECTS += 1;
    metric_tad* value = search_memory_metrics(memoryNumber);
    value->SELECTS += 1;
    pthread_mutex_unlock(&mutexMetrics);
}

void metric_insert(uint32_t memoryNumber) {
    pthread_mutex_lock(&mutexMetrics);
    TOTAL_INSERTS += 1;
    metric_tad* value = search_memory_metrics(memoryNumber);
    value->INSERTS += 1;
    pthread_mutex_unlock(&mutexMetrics);
}

void showMetrics (t_log* log_Kernel) {
    void log_memories(void* element) {
        metric_tad* m = element;
        log_info(log_Kernel, "Memory <%d> | Read <%d> / <%d>", m->MEMORY_NUMBER, m->SELECTS, TOTAL_SELECTS);
        log_info(log_Kernel, "Memory <%d> | Write <%d> / <%d>", m->MEMORY_NUMBER, m->INSERTS, TOTAL_INSERTS);
    }

    pthread_mutex_lock(&mutexMetrics);
    READ_LATENCY = average_time(LIST_READ_LATENCY_METRICS);
    WRITE_LATENCY = average_time(LIST_WRITE_LATENCY_METRICS);

    log_info(log_Kernel, "--------------------------------");
    log_info(log_Kernel, "Metricas");
    log_info(log_Kernel, "Read Latency <%f>", READ_LATENCY);
    log_info(log_Kernel, "Write Latency <%f>", WRITE_LATENCY);
    log_info(log_Kernel, "Read <%d>", TOTAL_SELECTS);
    log_info(log_Kernel, "Write <%d>", TOTAL_INSERTS);
    log_info(log_Kernel, "Memory Load");
    list_iterate(LIST_METRICS, (void*) log_memories);
    log_info(log_Kernel, "--------------------------------");
    clean_metrics();
    pthread_mutex_unlock(&mutexMetrics);
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

void finish_time_metric(time_metric_tad* value) {
    pthread_mutex_lock(&mutexMetrics);
    time(&value->end);
    value->difTime = difftime(value->end, value->start);
    pthread_mutex_unlock(&mutexMetrics);
}

time_metric_tad* start_time_read_metric() {
    pthread_mutex_lock(&mutexMetrics);
    time_metric_tad* time_metric = new_time_metric_value();
    list_add(LIST_READ_LATENCY_METRICS, time_metric);
    pthread_mutex_unlock(&mutexMetrics);
    return time_metric;
}

time_metric_tad* start_time_write_metric() {
    pthread_mutex_lock(&mutexMetrics);
    time_metric_tad* time_metric = new_time_metric_value();
    list_add(LIST_WRITE_LATENCY_METRICS, time_metric);
    pthread_mutex_unlock(&mutexMetrics);
    return time_metric;
}

double average_time(t_list* list) {
    if (list_size(list) == 0 ) {
        return 0;
    } else {
        double sumDif = 0;

        void _sum_dif(time_metric_tad value) {
            sumDif += value.difTime;
        }

        list_iterate(list, (void*) _sum_dif);

        return (sumDif / list_size(list));
    }
}