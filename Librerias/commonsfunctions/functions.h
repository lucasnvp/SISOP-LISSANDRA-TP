#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <inttypes.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/stat.h>

uint32_t bytesToMB(uint32_t bytes);
uint32_t mbToBytes(uint32_t mBytes);

uint32_t ValidarArchivo(char* path);
uint32_t sizeArchivo(char* path);
char* typeArchivo(char* path);

int remove_directory(const char *path);

#endif /* FUNCTIONS_H_ */
