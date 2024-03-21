// shmem-gen.c
// write a random number between 0 and 999 to the shm every 1 second
#include <stdio.h>
#include <unistd.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <time.h>
#include <error.h>
#include <string.h>

int main(){
  int shm_id;
  int *share;
  int num;

  srand(time(NULL));
  shm_id = shmget (0x2FF, getpagesize(), 0666 | IPC_CREAT);
  printf("shm_id = %d\n", shm_id);
  if(shm_id < 0){
    perror("shmget()");
    exit(1);
  }

  /* подключение сегмента к адресному пространству процесса */
  share = (int *)shmat(shm_id, 0, 0);
  if(share == NULL){
    perror("shmat()");
    exit(2);
  }
  printf("share = %p\n", share);
  int flag = 1;
  while(flag){
    num = random() % 1000;
    *share = num;
    printf("write a random number %d\n", num);
    printf("Enter 0 to finish, enter other number to continue: ");
    auto res = scanf("%d", &flag);
    res += 1;
  }
  *share = 1001;
  shmdt (share);
  if (shmctl (shm_id, IPC_RMID, (struct shmid_ds *) 0) < 0) {
        perror("eraser: shared memory remove error");
  }
  return 0;
}
