#ifndef POVM_LAMBDA_RESULT_H
#define POVM_LAMBDA_RESULT_H
#include <datum.h>

#define LAMBDA_TYPE_ERROR -1
#define LAMBDA_OPERANDS_NOT_COMPARABLE -2
#define LAMBDA_WRONG_OPERATOR -3
#define LAMBDA_INDEX_OUT_OF_BOUNDARY -4
#define LAMBDA_INVALID_OPERANDS -5
#define LAMBDA_UNDEFINED_BEHAVIOR -6

enum result_type {
    ERROR,
    RESULT
};

struct result {
    enum result_type type;
    union {
        long int error_code;
        struct datum datum;
    };
};

struct result result_from_error_code(long int error_code);
struct result result_from_datum(struct datum datum);

#endif
