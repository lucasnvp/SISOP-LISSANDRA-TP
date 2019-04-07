//
// Created by utnso on 06/04/19.
//

#ifndef TP_2019_1C_GANK_MID_FILESYSTEM_H
#define TP_2019_1C_GANK_MID_FILESYSTEM_H

#include <stdio.h>
#include <stdlib.h>

#include "config/Config_filesystem.h"

char* PATH_CONFIG = "/home/utnso/Gank-mid/tp-2019-1c-Gank-mid/FileSystem/src/config/config.txt";
Type_Config config;

char* PATH_LOG = "/home/utnso/Gank-mid/Logs/logFileSystem.txt";
t_log* log_Console;
t_log* log_FileSystem;

#endif //TP_2019_1C_GANK_MID_FILESYSTEM_H
