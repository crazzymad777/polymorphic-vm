#ifndef POVM_STACK_INTERFACE_H
#define POVM_STACK_INTERFACE_H

#include <povm.h>

struct povm_stack_interface {
    //int next(void* ctx, int N);
    int (*skip)(void* ctx, int N); // N>0 forward, N<0 backwards
    int (*put_type)(void* ctx, int32_t type);
    int (*put_value)(void* ctx, union udatum value);
    int32_t (*get_type)(void* ctx);
    union udatum (*get_value)(void* ctx);
    int (*put_datum)(void* ctx, struct datum datum);
    struct datum (*get_datum)(void* ctx);
};

#endif
