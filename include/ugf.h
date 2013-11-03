#ifndef UGF_H
#define UGF_H

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stddef.h>

typedef int (*generic_pred)(const void*);
typedef int (*generic_comp)(const void*, const void*);
typedef void (*generic_op)(void*);
typedef void* (*generic_cpy)(const void*);

#endif
