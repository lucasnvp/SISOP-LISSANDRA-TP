add_executable(FileSystem
        src/FileSystem.c
        src/FileSystem.h
        src/config/Config_filesystem.c
        src/config/Config_filesystem.h
        src/comandos/comandos.c
        src/comandos/comandos.h
        )

target_link_libraries (FileSystem PRIVATE Librerias)
target_link_libraries (FileSystem PRIVATE pthread)
target_link_libraries (FileSystem PRIVATE readline)