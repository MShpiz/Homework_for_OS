// out-shmem.c
// read from the shm every 1 second
#include<stdio.h>
#include<unistd.h>
#include<sys/shm.h>
#include<stdlib.h>
#include<error.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <signal.h>

void my_sigint_handler(int nsig){
  exit(0);
}

int main() {
  int shm_id;
  char gen_object[] = "gen-memory"; //  имя объекта
  printf("%d\n", getpid());
  printf("Press any key to continue");
  int res = scanf("");
  (void)signal(SIGUSR1, my_sigint_handler);

  //открыть объект
  if ( (shm_id = shm_open(gen_object, O_RDWR, 0666)) == -1 ) {
    printf("Opening error\n");
    perror("shm_open");
    return 1;
  } else {
    printf("Object is open: name = %s, id = 0x%x\n", gen_object, shm_id);
  }

  //получить доступ к памяти
  int* addr = (int*)mmap(0, sizeof(int), PROT_WRITE|PROT_READ, MAP_SHARED, shm_id, 0);
  if (addr == (int*)-1 ) {
    printf("Error getting pointer to shared memory\n");
    return 1;
  }

  while(1){
    
    printf("%d\n", *addr);
  }

  return 0;
}
