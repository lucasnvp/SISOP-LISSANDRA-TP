#ifndef TP_2019_1C_GANK_MID_SETUP_H
#define TP_2019_1C_GANK_MID_SETUP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>

#include <commons/bitarray.h>
#include <commons/config.h>
#include <commons/string.h>
#include "commons/config.h"

#include "../comandos/comandos.h"
#include "../config/Config_filesystem.h"
#include "../setup/setup.h"

static const char* TAMANIO_BLOQUE = "BLOCK_SIZE=";
static const char* CANTIDAD_BLOQUE = "BLOCKS=";
static const char* MAGIC_NUMBER = "MAGIC_NUMBER=LISSANDRA\n";

t_config * metadataConfig;
Type_Config* config;

FILE * metadata;
FILE * bitmap;

t_bitarray * bitarray;
char* mmapBitmap;

char* puntoDeMontaje;
char* montajeMetadata;
char* montajeTablas;
char* montajeBloques;

int i;

int CANT_BLOQUES;
int TAMANIO_BLOQUES;

int TAMANIO_VALUE;

void punto_montaje_setup(char* pdm);
void setup();
void carpetas_setup();
void metadata_setup(char* metadatabin);
void new_metadata_setup(char* metadatabin);
void bitmap_setup(char* bitmapbin);
void new_bitmap_setup(char* bitmapbin);
void bloques_setup();
void setUp_compactation_threads();

#endif //TP_2019_1C_GANK_MID_SETUP_H
