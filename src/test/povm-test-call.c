#include <povm-gen.h>
#include <munit.h>

void test_command_ret() {
    const char buffer[] = {COMMAND_RET};
    FILE* fd = fmemopen((void*)buffer, sizeof(buffer), "r");
    enum datum_type types[] = {};
    int64_t stack[] = {};

    int32_t* types_ptr = (int32_t*)&types;
    union udatum* udatum_stack = (union udatum*)stack;

    int r = povm_execute(fd, udatum_stack, types_ptr);
    munit_assert_int(r, ==, -42);
    fclose(fd);
}

void test_command_halt_call() {
    const char buffer[] = {COMMAND_CALL, 0, 0, 0, 0, 0, 0, 0, 0};
    FILE* fd = fmemopen((void*)buffer, sizeof(buffer), "r");
    enum datum_type types[] = {};
    int64_t stack[] = {};

    int32_t* types_ptr = (int32_t*)&types;
    union udatum* udatum_stack = (union udatum*)stack;

    int r = povm_execute(fd, udatum_stack, types_ptr);
    munit_assert_int(r, ==, -6);
    fclose(fd);
}


void test_command_halt_jump() {
    const char buffer[] = {COMMAND_JMP, 0, 0, 0, 0, 0, 0, 0, 0};
    FILE* fd = fmemopen((void*)buffer, sizeof(buffer), "r");
    enum datum_type types[] = {};
    int64_t stack[] = {};

    int32_t* types_ptr = (int32_t*)&types;
    union udatum* udatum_stack = (union udatum*)stack;

    int r = povm_execute(fd, udatum_stack, types_ptr);
    munit_assert_int(r, ==, -6);
    fclose(fd);
}
