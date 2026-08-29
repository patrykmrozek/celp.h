# celp.h
`celp.h` (c-help) is a single header generic C util library.

### Features
Dynamic Array - `celp_da`
Linked List - `celp_ll`
Hashmap - `celp_map`
Unit Testing Framework `CELP_TEST`
Linear Algebra `CELP_MATH`  (v2, v3, v4, m4)
Logging - `celp_log` (or helpful macros `CELP_INFO/DEBUG/ERROR/TRACE`)
Prefix stripping - `CELP_STRIP_PREFIX` (if you don't want to keep writing celp..)
Type aliases
Other useful utilities..

### Usage
```C
#define CELP_TEST/CELP_MATH (if needed)
#define CELP_STRIP_PREFIX (optional)
#define CELP_IMPLEMENTATION (define only once)
#include "celp.h"
...
```

work in progress
