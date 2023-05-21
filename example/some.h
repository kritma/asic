#include "../src/asic.h"

char *read_all_file(char *filename);

struct read_all_file_promise;
char *read_all_file_promise_wait(struct read_all_file_promise *promise);
struct read_all_file_promise *read_all_file_async(char *);
