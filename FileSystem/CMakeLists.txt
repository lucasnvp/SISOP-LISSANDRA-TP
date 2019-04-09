add_executable(FILESYSTEM
        src/FileSystem.c
        src/FileSystem.h
        src/config/Config_filesystem.c
        src/config/Config_filesystem.h
        src/comandos/comandos.c
        src/comandos/comandos.h
        )

target_link_libraries (FILESYSTEM PRIVATE Librerias)
target_link_libraries (FILESYSTEM PRIVATE pthread)
target_link_libraries (FILESYSTEM PRIVATE readline)