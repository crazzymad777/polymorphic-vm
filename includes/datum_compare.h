#ifndef LAMBDA_DATUM_COMPARE_H
#define LAMBDA_DATUM_COMPARE_H

#include "datum.h"
#include <stdbool.h>
// #define LAMBDA_COMPARE_BOOLEAN_TO_INTEGER 1
// #define LAMBDA_COMPARE_BOOLEAN_TO_FLOATING 1


enum OP_COMPARE {
    OP_COMPARE_LESS = -9223372036854775808,
    OP_COMPARE_EQUALS = 0,
    OP_COMPARE_MORE = 2,
    OP_COMPARE_NOT_EQUALS = 1
};

// If at least one of operands is VOID call this function
enum OP_COMPARE datum_op_compare_void(struct datum p0, struct datum p1);
// If at least one of operands is BOOLEAN call this function
bool datum_op_equals_boolean(struct datum p0, struct datum p1);
enum OP_COMPARE datum_op_compare_boolean(struct datum p0, struct datum p1);
// If at least one of operands is STRING call this function
// bool datum_op_compare_string(struct datum p0, struct datum p1); // body in datum_string_impl.c

#endif
