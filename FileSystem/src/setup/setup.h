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

static const char* TAMANIO_BLOQUE = "TAMANIO_BLOQUES=64\n";
static const char* CANTIDAD_BLOQUE = "CANTIDAD_BLOQUES=10\n";
static const char* MAGIC_NUMBER = "MAGIC_NUMBER=LISSANDRA\n";

t_config * metadataConfig;

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

void punto_montaje_setup(char* pdm);
void setup();
void carpetas_setup();
void metadata_setup(char* metadatabin);
void new_metadata_setup(char* metadatabin);
void bitmap_setup(char* bitmapbin);
void new_bitmap_setup(char* bitmapbin);
void bloques_setup();

#endif //TP_2019_1C_GANK_MID_SETUP_H
