// shmem-out.c
// read from the shm every 1 second
#include<stdio.h>
#include<unistd.h>
#include<sys/shm.h>
#include<stdlib.h>
#include<error.h>

int main() {
  int shm_id;
  int *share;

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
  bool flag = true;
  while(flag){
    sleep(1);
    if (*share == 1001){
      break;
    }
    printf("%d\n", *share);
  }

  shmdt (share);

  return 0;
}
