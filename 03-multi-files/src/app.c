#include <stdio.h>

int add(int, int);

int main(void) {
  printf("1+1=%d", add(1, 1));
  return 0;
}

int add(int a, int b) {
  return a + b;
}