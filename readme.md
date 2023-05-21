# Asic - easy async
### **asic** uses macros to generate async functions

**asic()** generates necessary functions to work with
```cpp
#define asic(func, return_type, args_types...)
```

**asicDefinition()** generates definitions for promise type, wait and async functions;
```cpp
#define asicDefinition(func, return_type, args_types...)
```

```cpp
char *read_all_file(char *filename);

asicDefinition(read_all_file, char *, char *);
```
expands to
```cpp
char *read_all_file(char *filename);

struct read_all_file_promise;
struct read_all_file_promise *read_all_file_async(char *);
char *read_all_file_promise_wait(struct read_all_file_promise *promise);
```

## drawbacks
- only **gcc** (because of use __builtin_apply_args, __builtin_apply)
- max 1024 bytes for arguments or less (idk how to calc size of args_types)