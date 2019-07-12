//
// Created by utnso on 28/05/19.
//

#ifndef TP_2019_1C_GANK_MID_METRICAS_H
#define TP_2019_1C_GANK_MID_METRICAS_H

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <time.h>
#include <pthread.h>

#include <commons/log.h>
#include <commons/collections/list.h>

typedef struct {
    uint32_t MEMORY_NUMBER;
    uint32_t SELECTS;
    uint32_t INSERTS;
} metric_tad;

typedef struct {
    time_t start;
    time_t end;
    double difTime;
} time_metric_tad;

pthread_mutex_t mutexMetrics;
t_list * LIST_METRICS;
t_list * LIST_READ_LATENCY_METRICS;
t_list * LIST_WRITE_LATENCY_METRICS;
uint32_t TOTAL_INSERTS;
uint32_t TOTAL_SELECTS;
double READ_LATENCY;
double WRITE_LATENCY;

void init_metrics();
metric_tad* new_metric_value(uint32_t memoryNumber);
time_metric_tad* new_time_metric_value();
void free_metric_value(metric_tad* value);
void free_time_metric_value(time_metric_tad* value);
void clean_metrics();
void metric_select(uint32_t memoryNumber);
void metric_insert(uint32_t memoryNumber);
void showMetrics(t_log* log_Kernel);
metric_tad* search_memory_metrics(uint32_t number);
void finish_time_metric(time_metric_tad* value);
time_metric_tad* start_time_read_metric();
time_metric_tad* start_time_write_metric();
double average_time(t_list* list);

#endif //TP_2019_1C_GANK_MID_METRICAS_H
