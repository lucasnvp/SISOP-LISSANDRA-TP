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

        commonsfunctions/functions.c
        commonsfunctions/functions.h

        serializador/estructuras.c
        serializador/estructuras.h
        serializador/serializador.c
        serializador/serializador.h

        servidor/servidor.c
        servidor/servidor.h

        parser/parser.c
        parser/parser.h serializador/handshake.h)

TARGET_INCLUDE_DIRECTORIES(Librerias PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")