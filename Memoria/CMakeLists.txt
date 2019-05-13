add_executable(Memoria
        src/Memoria.c
        src/Memoria.h
        src/config/Config_memoria.c
        src/config/Config_memoria.h
        src/comandos/comandos.c src/comandos/comandos.h src/estructuras/paginas.c src/estructuras/paginas.h src/estructuras/segmentos.c src/estructuras/segmentos.h)

target_link_libraries (Memoria PRIVATE Librerias)
target_link_libraries (Memoria PRIVATE pthread)
target_link_libraries (Memoria PRIVATE readline)