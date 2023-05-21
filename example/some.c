#include "some.h"
#include "../src/asic.h"
#include <stdio.h>

char *read_all_file(char *filename) {
  FILE *file = fopen(filename, "r");

  if (file == NULL) {
    return NULL;
  }

  fseek(file, 0L, SEEK_END);
  long file_size = ftell(file);
  rewind(file);

  char *txt = malloc(file_size + 1);
  fread(txt, sizeof(char), file_size, file);
  txt[file_size] = 0;
  return txt;
}

asic(read_all_file, char *, char *)
