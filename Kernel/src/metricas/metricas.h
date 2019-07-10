//
// Created by utnso on 28/05/19.
//

#ifndef TP_2019_1C_GANK_MID_METRICAS_H
#define TP_2019_1C_GANK_MID_METRICAS_H

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#include <commons/log.h>
#include <commons/collections/list.h>

typedef struct {
    uint32_t MEMORY_NUMBER;
    uint32_t SELECTS;
    uint32_t INSERTS;
} metric_tad;

t_list * LIST_METRICS;
uint32_t TOTAL_INSERTS;
uint32_t TOTAL_SELECTS;

void init_metrics();
metric_tad* new_metric_value(uint32_t memoryNumber);
void free_metric_value(metric_tad* value);
void metric_select(uint32_t memoryNumber);
void metric_insert(uint32_t memoryNumber);
void showMetrics(t_log* log_Kernel);
metric_tad* search_memory_metrics(uint32_t number);

#endif //TP_2019_1C_GANK_MID_METRICAS_H
