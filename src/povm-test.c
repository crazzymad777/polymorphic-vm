#include <povm-gen.h>
#include <munit.h>


int test_command_add_i64(int64_t a, int64_t b) {
    const char buffer[] = {COMMAND_ADD};
    FILE* fd = fmemopen((void*)buffer, sizeof(buffer), "r");
    enum datum_type types[] = {I64, I64};
    int64_t stack[] = {a, b};

    int32_t* types_ptr = (int32_t*)&types;
    union udatum* udatum_stack = (union udatum*)stack;
    udatum_stack += 1;
    types_ptr += 1;

    povm_execute(fd, udatum_stack, types_ptr);
    munit_assert_int(stack[0], ==, a + b);
    fclose(fd);
}

int main() {
    // We must test each command
    // We must test generator
    test_command_add_i64(25, 42);
    return 0;
}
