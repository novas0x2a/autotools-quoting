#include <stdio.h>
#include <string.h>

int main(int argc, char** argv) {
  char actual[] = __DATE__;
  char expected[] = "redacted";

  if (0 == strncmp(actual, expected, sizeof(expected))) {
    printf("everything in its right place\n");
    return 0;
  }

  printf("__DATE__ supposed to be '%s', but is '%s'\n", expected, actual);

  return 1;
}
