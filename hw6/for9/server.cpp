// shmem-out.c
// read from the shm every 1 second
#include<stdio.h>
#include<unistd.h>
#include<sys/shm.h>
#include<stdlib.h>
#include<error.h>
#include<signal.h>

int *share;

void my_sigint_handler(int nsig){
  exit(0);
}

int main() {
  int shm_id;
  

  printf("%d\n", getpid());
  (void)signal(SIGUSR1, my_sigint_handler);

  shm_id = shmget (0x2FF, getpagesize(), 0666 | IPC_CREAT);
  if(shm_id == -1){
    perror("shmget()");
    exit(1);
  }

  share = (int *)shmat(shm_id, 0, 0);
  if(share == NULL){
    perror("shmat()");
    exit(2);
  }
  int* prev = new int(-1);
  while(true){
    printf("%d\n", *share);
  }

  

  return 0;
}
