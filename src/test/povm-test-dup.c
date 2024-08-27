#include <povm-gen.h>
#include <munit.h>

int test_command_push_i64(int64_t a) {
    enum datum_type type = I64;
    char buffer[13] = {COMMAND_PUSH};
    memcpy((void*)&buffer[1], (void*)&type, 4);
    memcpy((void*)&buffer[5], (void*)&a, 8);
    FILE* fd = fmemopen((void*)buffer, sizeof(buffer), "r");
    enum datum_type types[] = {0};
    int64_t stack[] = {0};

    int32_t* types_ptr = (int32_t*)&types;
    union udatum* udatum_stack = (union udatum*)stack;

    povm_execute(fd, udatum_stack-1, types_ptr-1);
    munit_assert_int(stack[0], ==, a);
    munit_assert_int(types[0], ==, I64);
    fclose(fd);
}

int test_command_dup_i64(int64_t a) {
    const char buffer[] = {COMMAND_DUP};
    FILE* fd = fmemopen((void*)buffer, sizeof(buffer), "r");
    enum datum_type types[] = {I64, 0};
    int64_t stack[] = {a, 0};

    int32_t* types_ptr = (int32_t*)&types;
    union udatum* udatum_stack = (union udatum*)stack;

    povm_execute(fd, udatum_stack, types_ptr);
    munit_assert_int(stack[0], ==, stack[1]);
    munit_assert_int(types[0], ==, types[1]);
    fclose(fd);
}

int test_command_swap_i64(int64_t a, int64_t b) {
    const char buffer[] = {COMMAND_SWAP};
    FILE* fd = fmemopen((void*)buffer, sizeof(buffer), "r");
    enum datum_type types[] = {I64, I64};
    int64_t stack[] = {a, b};

    int32_t* types_ptr = (int32_t*)&types;
    union udatum* udatum_stack = (union udatum*)stack;

    types_ptr += 1;
    udatum_stack += 1;

    povm_execute(fd, udatum_stack, types_ptr);

    munit_assert_int(stack[0], ==, b);
    munit_assert_int(stack[1], ==, a);
    fclose(fd);
}
