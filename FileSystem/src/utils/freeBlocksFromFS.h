//
// Created by utnso on 16/06/19.
//

#ifndef TP_2019_1C_GANK_MID_FREEBLOCKSFROMFS_H
#define TP_2019_1C_GANK_MID_FREEBLOCKSFROMFS_H

#include <commons/log.h>
#include "utils.h"

t_log* log_FileSystem_Free_Blocks;

void* freeBlocksFromTemps(char* table, char* typeFile);
void* freeBlocksFromFS(char* table);
void* limpiarYLiberarBloque(char* path);

#endif //TP_2019_1C_GANK_MID_FREEBLOCKSFROMFS_H