#include "some.h"
#include "unistd.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  char *file_name = "example/main.c";

  struct read_all_file_promise *promise = read_all_file_async(file_name);
  // cpu intense work
  printf("hard work started\n");
  usleep(1000 * 1000);
  printf("hard work done\n");

  char *txt = read_all_file_promise_wait(promise);
  if (txt == NULL) {
    char full_path[500];
    realpath(file_name, full_path);
    printf("no such file!? %s\n", full_path);
    return -1;
  }
  printf("file:\n%s\n", txt);
  return 0;
}
