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

/*!
   \file ugf.h
   \brief Universal Generic Functions header file.
   \details Header file denoting typedef forms of all general generic functions
   typically used in data structures.
   \author Timothy Bullard
   \version 1.0
 */

/*!
   \var typedef int (*generic_pred)(const void*)
   \brief Typedef form of generic predicate functions.
   \param Pointer to data being examined.
   \return Integer value 1 representing if data is "true" with respects to the predicate,
   and 0 if "false".
   \details An example using ints that checks to see if the value is greater than 5:
      \code{.c}
int integer_pred(const void* int1) {
    int pred_value;
    pred_value = 0;
    if(*(int*)int1 > 5) {
        pred_value = 1;
    }
    return pred_value;
}
   \endcode
 */

/*!
   \var typedef int (*generic_comp)(const void*, const void*)
   \brief Typedef form of generic comparison fuctions.
   \param Pointer to \a data-1 being examined.
   \param Pointer to \a data-2 being examined.
   \return Integer value 1 if \a data-1 is "greater than" \a data-2,
   0 if \a data-1 and \a data-2 are "equal", and -1 if \a data-1 is "less than" \a data-2.
   \details An example using ints:
   \code{.c}
int integer_comp(const void* int1, const void* int2) {
    int comp_value;
    comp_value = 0;
    if(*(int*)int1 > *(int*)int2) {
        comp_value = 1;
    } else if(*(int*)int1 < *(int*)int2) {
        comp_value = -1;
    }
    return comp_value;
}
   \endcode
 */

/*!
   \var typedef void (*generic_op)(void*)
   \brief Typedef form of generic operation functions.
   \param Pointer to data being operated upon.
   \details An example using ints:
   \code{.c}
void integer_op(void* int1) {
    free(int1);
    return;
}
   \endcode
 */

/*!
   \var typedef void* (*generic_cpy)(const void*);
   \brief Typedef form of generic copy functions.
   \param Point to data being copied.
   \return Pointer to newly allocated copy of input.
   \details An example using ints:
   \code{.c}
void* integer_copy(const void* int1) {
    int* copy_value;
    copy_value = malloc(sizeof(int));
    (*copy_value) = *(int*)int1;
    return copy_value;
}
   \endcode
 */
