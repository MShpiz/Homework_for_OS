#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>

pid_t pid;
pid_t other_pid;
int number = 0;

void sendCurrResponse() {
  kill(other_pid, SIGUSR1);
}

void gotOne(int nsig) {
  number <<= 1;
  number += 1;
  printf("got 1\n");
  sendCurrResponse();
}

void GotZero(int nsig) { 
  number <<= 1; 
  printf("got 0\n");
  sendCurrResponse();
}

void finnish(int nsig) {
  printf("number: %d\n", number);
   kill(pid, SIGKILL);
}

int main(void) {
  pid_t pid = getpid();
  printf("my pid %d\n", pid);

  printf("other sending process pid:");
  int a = scanf("%d", &other_pid);
  (void)signal(SIGUSR1, GotZero);
  (void)signal(SIGUSR2, gotOne);
  (void)signal(SIGPWR, finnish);
 
  while (1){}
  return 0;
}
