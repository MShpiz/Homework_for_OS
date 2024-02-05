#include <dirent.h>
#include <stdio.h>
#include <string>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;

// функция считающая факториал
// при переполнении возвращает последнее корректное значение
unsigned long int factorial(int num) {
  if (num < 1) {
    printf("bad argument for factorial\n");
    return 0;
  }
  unsigned long int result = 1;
  for (unsigned long int i = 1; i <= num; i++) {
    if (result * i < result) {
      printf("Overflow in factorial occured. Returning last correct result\n");
      break;
    }

    result *= i;
  }
  return result;
}

// функция считающая числа фибоначи
// при переполнении возвращает последнее корректное значение
unsigned long int fibo(int num) {
  if (num < 0) {
    printf("bad argument for fibo\n");
    return 0;
  }
  unsigned long int prev = 1;
  unsigned long int result = 1;
  for (unsigned long int i = 1; i < num; i++) {
    if (result + prev < result) {
      printf("Overflow in fibo occured. Returning last correct result\n");
      break;
    }

    unsigned long int tmp = result;
    result += prev;
    prev = tmp;
  }
  return result;
}

// функция выводящая информацию о каталоге
void listCatalogue() {
  DIR *dir;
  struct dirent *ent;
  if ((dir = opendir("./")) != NULL) {
    while ((ent = readdir(dir)) != NULL) {
      printf("%s\n", ent->d_name);
    }
    closedir(dir);
  } else {
    printf("something went wrong");
  }
}

int main(int argc, char *argv[]) {

  pid_t pid, ppid, chpid, other_chpid;
  chpid = fork(); // делаем первого ребёнка
  pid = getpid();
  ppid = getppid();

  if (chpid <= -1) {
    printf("Incorrect fork syscall\n");
  } else if (chpid == 0) { // если поток ребёнок запускаем факториал
    printf("I am child. \nMy pid = %d, my parent is %d and I have no child\nI "
           "count factorial\n",
           (int)pid, (int)ppid);
    unsigned long int res1 = factorial(atoi(argv[1]));
    printf("factorial result: %lu \n", res1);
  } else { // если поток родитель запускаем фибоначи
    printf("I am parent. \nMy pid = %d, my parent is %d, my child is %d\nI "
           "count Fibo\n",
           (int)pid, (int)ppid, (int)chpid);
    unsigned long int res2 = fibo(atoi(argv[1]));
    printf("fibo result: %lu \n", res2);
  }

  if (chpid != 0) { // если поток родитель делает другого ребёнка
    wait(NULL);
    other_chpid = fork();
  } 

  if (chpid != 0 && other_chpid != 0) {
    printf("I am parent. \nMy pid = %d, my parent is %d, I made another child, it is %d\nI "
     "count Fibo\n",
     (int)pid, (int)ppid, (int)other_chpid);
  }

  if (other_chpid == 0 && chpid != 0) {// если поток другой ребёнок запускаем просмотр каталога
    pid = getpid();
    ppid = getppid();
    printf("I am other child. \nMy pid = %d, my parent is %d and I have no "
           "child\nI write what is in the catalogue\n",
           (int)pid, (int)ppid);
    listCatalogue();
  }

  return 0;
}
