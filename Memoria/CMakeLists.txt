add_executable(Memoria
        src/Memoria.c
        src/Memoria.h
        src/config/Config_memoria.c
        src/config/Config_memoria.h
        )

target_link_libraries (Memoria PRIVATE Librerias)