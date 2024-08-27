#include <povm-gen.h>
#include <munit.h>

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
