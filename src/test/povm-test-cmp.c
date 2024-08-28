#include <povm-gen.h>
#include <string.h>
#include <munit.h>

void test_command_cmp_eq() {
    // ret, EOF
    const char buffer[] = {COMMAND_CMP};
    FILE* fd = fmemopen((void*)buffer, sizeof(buffer), "r");
    enum datum_type types[] = {I64, F64, VOID};
    int64_t stack[3];

    int32_t* types_ptr = (int32_t*)&types;
    union udatum* udatum_stack = (union udatum*)stack;
    udatum_stack[0].i64 = 5;
    udatum_stack[1].f64 = 5.0;

    types_ptr += 1;
    udatum_stack += 1;

    int r = povm_execute(fd, udatum_stack, types_ptr);
    munit_assert_int(types[2], ==, COMPARE_RESULT);
    munit_assert_int(stack[2], ==, 0);
    fclose(fd);
}

#include <math.h>

void test_command_cmp_not_eq() {
    // ret, EOF
    const char buffer[] = {COMMAND_CMP};
    FILE* fd = fmemopen((void*)buffer, sizeof(buffer), "r");
    enum datum_type types[] = {F64, F64, VOID};
    int64_t stack[3];

    int32_t* types_ptr = (int32_t*)&types;
    union udatum* udatum_stack = (union udatum*)stack;
    udatum_stack[0].f64 = NAN;
    udatum_stack[1].f64 = 5.0;

    types_ptr += 1;
    udatum_stack += 1;

    int r = povm_execute(fd, udatum_stack, types_ptr);
    munit_assert_int(types[2], ==, COMPARE_RESULT);
    munit_assert_int(stack[2], ==, 1);
    fclose(fd);
}


void test_command_cmp_gt() {
    // ret, EOF
    const char buffer[] = {COMMAND_CMP};
    FILE* fd = fmemopen((void*)buffer, sizeof(buffer), "r");
    enum datum_type types[] = {F64, F64, VOID};
    int64_t stack[3];

    int32_t* types_ptr = (int32_t*)&types;
    union udatum* udatum_stack = (union udatum*)stack;
    udatum_stack[0].f64 = 5,0; // right side operand
    udatum_stack[1].f64 = 25.0; // left side operand

    types_ptr += 1;
    udatum_stack += 1;

    int r = povm_execute(fd, udatum_stack, types_ptr);
    munit_assert_int(types[2], ==, COMPARE_RESULT);
    munit_assert_int(stack[2], ==, 1 | 2);
    fclose(fd);
}


void test_command_cmp_lt() {
    // ret, EOF
    const char buffer[] = {COMMAND_CMP};
    FILE* fd = fmemopen((void*)buffer, sizeof(buffer), "r");
    enum datum_type types[] = {F64, F64, VOID};
    int64_t stack[3];

    int32_t* types_ptr = (int32_t*)&types;
    union udatum* udatum_stack = (union udatum*)stack;
    udatum_stack[0].f64 = 42.0;
    udatum_stack[1].f64 = 5.0;

    types_ptr += 1;
    udatum_stack += 1;

    int r = povm_execute(fd, udatum_stack, types_ptr);
    munit_assert_int(types[2], ==, COMPARE_RESULT);
    int64_t expected = 1ll | 1ll << 31 | 1ll << 63;
    munit_assert_int64(stack[2], ==, expected);
    fclose(fd);
}
