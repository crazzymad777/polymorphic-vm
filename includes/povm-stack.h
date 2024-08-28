#ifndef POVM_STACK_H
#define POVM_STACK_H

#include <povm-stack-interface.h>

struct povm_stack_interface* povm_get_std_stack_interface();

struct povm_stack {
    struct povm_stack_interface* interface;
    int32_t* types;
    union udatum* stack;
};

#endif
