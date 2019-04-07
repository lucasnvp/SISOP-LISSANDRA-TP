add_library (Librerias STATIC
        commons/collections/dictionary.h
        commons/collections/dictionary.c
        commons/collections/list.h
        commons/collections/list.c
        commons/collections/node.h
        commons/collections/queue.h
        commons/collections/queue.c
        commons/bitarray.h
        commons/bitarray.c
        commons/config.h
        commons/config.c
        commons/error.h
        commons/error.c
        commons/log.h
        commons/log.c
        commons/process.h
        commons/process.c
        commons/string.h
        commons/string.c
        commons/temporal.h
        commons/temporal.c
        commons/txt.h
        commons/txt.c
        )

TARGET_INCLUDE_DIRECTORIES(Librerias PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")