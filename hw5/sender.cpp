#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>

int currByteCount = 31;
pid_t pid;
pid_t other_pid;
int number;
bool bits[32];

void makeBitArray() {
  for (int i = 31; i >= 0; i--) {
    bits[i] = (number & (1 << i)) != 0;
  }
}

void sendCurrByte() {
  if (currByteCount < 0) {
    kill(other_pid, SIGPWR);
    kill(pid, SIGKILL);
    return;
  }
  if (!bits[currByteCount]) {
    kill(other_pid, SIGUSR1);
    printf("sent 0\n");
  } else {
    kill(other_pid, SIGUSR2);
    printf("sent 1\n");
  }
}

void gotNumber(int nsig) {
  currByteCount--;
  sendCurrByte();
}

int main(void) {
  pid_t pid = getpid();
  printf("my pid %d\n", pid);

  printf("other recieving process pid:");
  int a = scanf("%d", &other_pid);

  printf("enter number: ");
  a = scanf("%d", &number);
  (void)signal(SIGUSR1, gotNumber);
  makeBitArray();
  sendCurrByte();
  while (1);
  
  return 0;
}
