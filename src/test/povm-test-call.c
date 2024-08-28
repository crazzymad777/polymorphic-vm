#include <povm-gen.h>
#include <string.h>
#include <munit.h>

void test_command_ret() {
    // ret, EOF
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
    // call 0x0
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
    // jmp 0x0
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

void test_command_jump_to_eof(int64_t x) {
    // x is garbahe which PoVM must ignore
    // jmp 17
    // x...
    // EOF
    char buffer[9 + 8] = {COMMAND_JMP};
    int64_t offset = 1 + 8 + 8; // opcode + offset + garbage
    memcpy(&buffer[1], &offset, 8);
    memcpy(&buffer[9], &x, 8);
    FILE* fd = fmemopen((void*)buffer, sizeof(buffer), "r");
    enum datum_type types[] = {};
    int64_t stack[] = {};

    int32_t* types_ptr = (int32_t*)&types;
    union udatum* udatum_stack = (union udatum*)stack;

    int r = povm_execute(fd, udatum_stack, types_ptr);
    munit_assert_int(r, ==, 0);
    fclose(fd);
}

void test_command_jump_call() {
    // call label
    // ret
    // label: ret
    char buffer[11] = {COMMAND_CALL};
    int64_t offset = 1 + 8 + 9;
    memcpy(&buffer[1], &offset, 8);
    buffer[9] = COMMAND_RET;
    buffer[10] = COMMAND_RET;
    FILE* fd = fmemopen((void*)buffer, sizeof(buffer), "r");
    enum datum_type types[] = {};
    int64_t stack[] = {};

    int32_t* types_ptr = (int32_t*)&types;
    union udatum* udatum_stack = (union udatum*)stack;

    int r = povm_execute(fd, udatum_stack, types_ptr);
    munit_assert_int(r, ==, -42);
    fclose(fd);
}
