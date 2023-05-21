#include <stdlib.h>
#include <string.h>
#include <threads.h>

#define asicDefinition(func, return_type, args_types...)                       \
  struct func##_promise;                                                       \
  struct func##_promise *func##_async(args_types);                             \
  return_type func##_promise_wait(struct func##_promise *promise);

#define asic(func, return_type, args_types...)                                 \
  struct func##_promise {                                                      \
    cnd_t condition;                                                           \
    mtx_t mutex;                                                               \
    return_type value;                                                         \
    int is_done;                                                               \
  };                                                                           \
  struct func##_worker_arg {                                                   \
    struct func##_promise *promise;                                            \
    void *arg;                                                                 \
  };                                                                           \
  return_type func##_promise_wait(struct func##_promise *promise) {            \
    mtx_lock(&promise->mutex);                                                 \
    if (promise->is_done == 0) {                                               \
      cnd_wait(&promise->condition, &promise->mutex);                          \
    }                                                                          \
    mtx_unlock(&promise->mutex);                                               \
    return_type value = promise->value;                                        \
    free(promise);                                                             \
    return value;                                                              \
  }                                                                            \
  int func##_worker(void *arg) {                                               \
    struct func##_worker_arg *args = (struct func##_worker_arg *)arg;          \
    mtx_lock(&args->promise->mutex);                                           \
    args->promise->value =                                                     \
        *(return_type *)__builtin_apply((void (*)())func, args->arg, 1024);    \
    args->promise->is_done = 1;                                                \
    cnd_signal(&args->promise->condition);                                     \
    mtx_unlock(&args->promise->mutex);                                         \
    free(args->arg);                                                           \
    free(args);                                                                \
    return 0;                                                                  \
  }                                                                            \
  struct func##_promise *func##_async(args_types) {                            \
    void *arg = __builtin_apply_args();                                        \
    struct func##_promise *promise = malloc(sizeof(struct func##_promise));    \
    cnd_init(&promise->condition);                                             \
    mtx_init(&promise->mutex, mtx_plain);                                      \
    promise->is_done = 0;                                                      \
    struct func##_worker_arg *worker_arg =                                     \
        malloc(sizeof(struct func##_worker_arg));                              \
    worker_arg->arg = malloc(1024);                                            \
    memcpy(worker_arg->arg, arg, 1024);                                        \
    worker_arg->promise = promise;                                             \
    thrd_t thread;                                                             \
    thrd_create(&thread, func##_worker, worker_arg);                           \
    return promise;                                                            \
  }
