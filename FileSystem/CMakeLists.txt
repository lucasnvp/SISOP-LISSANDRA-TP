add_executable(FileSystem
        src/FileSystem.c
        src/FileSystem.h
        src/config/Config_filesystem.c
        src/config/Config_filesystem.h
        src/comandos/comandos.c
        src/comandos/comandos.h
        src/setup/setup.c
        src/setup/setup.h
        src/utils/utils.c
        src/utils/utils.h src/utils/memtableHandler.c src/utils/memtableHanlder.h src/utils/tablesHandler.c src/utils/tablesHandler.h src/utils/getterValuesFromFS.c src/utils/getterValuesFromFS.h src/utils/compactator.c src/utils/compactator.h src/utils/freeBlocksFromFS.c src/utils/freeBlocksFromFS.h)

target_link_libraries (FileSystem PRIVATE Librerias)
target_link_libraries (FileSystem PRIVATE pthread)
target_link_libraries (FileSystem PRIVATE readline)