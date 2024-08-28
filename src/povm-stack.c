#include <povm-stack.h>

int skip(void* ctx, int N) {
    struct povm_stack* s = ctx;
    s->stack += N;
    s->types += N;
    return 0;
}

int put_type(void* ctx, int32_t type) {
    struct povm_stack* s = ctx;
    *(s->types) = type;
    return 0;
}

int put_value(void* ctx, union udatum value) {
    struct povm_stack* s = ctx;
    *(s->stack) = value;
    return 0;
}

int32_t get_type(void* ctx) {
    struct povm_stack* s = ctx;
    return *(s->types);
}

union udatum get_value(void* ctx) {
    struct povm_stack* s = ctx;
    return *(s->stack);
}

int put_datum(void* ctx, struct datum datum) {
    struct povm_stack* s = ctx;
    struct povm_stack_interface* i = s->interface;
    int e = i->put_type(ctx, datum.type);
    if (e != 0) return e;
    return i->put_value(ctx, datum.udatum);
}

struct datum get_datum(void* ctx) {
    struct povm_stack* s = ctx;
    struct povm_stack_interface* i = s->interface;
    // povm_datum()
    struct datum d = {i->get_type(s), .udatum = i->get_value(s)};
    return d;
}

struct povm_stack_interface* povm_get_std_stack_interface() {
    static struct povm_stack_interface i;
    i.skip = skip;
    i.put_type = put_type;
    i.put_value = put_value;
    i.get_type = get_type;
    i.get_value = get_value;
    i.get_datum = get_datum;
    i.put_datum = put_datum;
    return &i;
}
