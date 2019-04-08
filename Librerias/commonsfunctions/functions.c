#include "functions.h"

uint32_t bytesToMB(uint32_t bytes){
	return bytes / 1048576;
}

uint32_t mbToBytes(uint32_t mBytes){
	return mBytes * 1048576;
}

uint32_t ValidarArchivo(char* path){
	uint32_t archivo_existente;
	if(access(path, F_OK) != -1){
		archivo_existente = true;
	} else{
		archivo_existente = false;
	}
	return archivo_existente;
}

uint32_t sizeArchivo(char* path){
	struct stat pathStat;
	int32_t fd = open(path,O_RDWR);
	fstat(fd,&pathStat);
	close(fd);
	return pathStat.st_size;
}

char* typeArchivo(char* path){
	char** type = string_split(path, ".");
	return type[1];
}

int remove_directory(const char *path){
   DIR *d = opendir(path);
   size_t path_len = strlen(path);
   int r = -1;

   if (d)
   {
      struct dirent *p;

      r = 0;

      while (!r && (p=readdir(d)))
      {
          int r2 = -1;
          char *buf;
          size_t len;

          /* Skip the names "." and ".." as we don't want to recurse on them. */
          if (!strcmp(p->d_name, ".") || !strcmp(p->d_name, ".."))
          {
             continue;
          }

          len = path_len + strlen(p->d_name) + 2;
          buf = malloc(len);

          if (buf)
          {
             struct stat statbuf;

             snprintf(buf, len, "%s/%s", path, p->d_name);

             if (!stat(buf, &statbuf))
             {
                if (S_ISDIR(statbuf.st_mode))
                {
                   r2 = remove_directory(buf);
                }
                else
                {
                   r2 = unlink(buf);
                }
             }

             free(buf);
          }

          r = r2;
      }

      closedir(d);
   }

   if (!r)
   {
      r = rmdir(path);
   }

   return r;
}
