#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

const int size = 32;

// Циклическое чтение с использованием системного вызова read
// Например, когда буфер небольшой
int main(int argv, char* args[]) {
  int     fd;
  int     fd2;
  char    buffer[size+1];
  ssize_t read_bytes;  
  ssize_t write_bytes;
  size_t  bytes_read;

  // получаем файловый дескриптор на чтение первого файла
  if((fd = open(args[1], O_RDONLY)) < 0){
    printf("Can\'t open file\n");
    exit(-1);
  }
   // получаем файловый дескриптор на запись во второй файл
  if((fd2 = open(args[2], O_WRONLY, 0666)) < 0){
    printf("Can\'t open file\n");
    exit(-1);
  }

  do {
    
    read_bytes = read(fd, buffer, size);
    if(read_bytes == -1) {
      printf("Can\'t read this file\n");
      exit(-1);
    }
    // читаем часть первого файла
    
    buffer[read_bytes] = '\0';
    // printf("%s", buffer);

    // записываем эту часть во второй файл
    write_bytes = write(fd2, buffer, read_bytes);
    if(write_bytes == -1) {
      printf("Can\'t write this file\n");
      exit(-1);
    }
  } while(read_bytes == size); // читаем пока буфер полный

 
  // закрываем файлы
  if(close(fd) < 0) {
    printf("Can\'t close reading file\n");
  }

  if(close(fd2) < 0) {
    printf("Can\'t close writing file\n");
  }

  return 0;
}
