#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int tmp; 
int fd[2];
int   semid;
struct sembuf mybuf;
char result[20] = "\0";

void child(int* pd, int semid) {
  tmp = read(pd[0], result, 15);
  printf("CHILD: %s\n", result);

  struct sembuf child_buf =
            {.sem_num = 0, .sem_op = 1, .sem_flg = 0};
  if(semop(semid, &child_buf, 1) < 0){
    printf("Can\'t add 1 to semaphor\n");
    exit(-1);
  }
  tmp = write(pd[1], "Child to parent", 15);
}

void parent(int* pd, int semid) {
  tmp = write(pd[1], "Parent to child", 15);

  struct sembuf parent_buf =
            {.sem_num = 0, .sem_op = -1, .sem_flg = 0};
  if(semop(semid, &parent_buf, 1) < 0){
    printf("Can\'t sub 1 from semaphor\n");
    exit(-1);
  }

  tmp = read(pd[0], result, 15);
  printf("PARENT: %s\n", result);
}

void finish(int sigino) {
  if (semctl(semid, 0, IPC_RMID, 0) < 0) {
    printf("Can\'t delete semaphore\n");
  }
  printf("Exit");
  exit(0);
}

int main() {
  signal(SIGINT, finish);

  if(pipe(fd) < 0) {
    printf("Pipe error\n");
    return -1;
  }

  char pathname[]="pipesem.c";
  key_t key = ftok(pathname, 0);

  if((semid = semget(key, 1, 0666 | IPC_CREAT | IPC_EXCL) ) < 0){
    printf("Can\'t create semaphore\n");
    return -1;
  }
  semctl(semid, 0, SETVAL, 0);

  while (true){
    pid_t pid = fork();
    if (pid < 0) {
      printf("Fork error\n");
      return -1;
    } else if (pid == 0) {
      sleep(1);
      child(fd, semid);
    } else {
      sleep(1);
      parent(fd, semid); 
    }
    close(fd[0]);
    close(fd[1]);
  }
  return 0;
}
