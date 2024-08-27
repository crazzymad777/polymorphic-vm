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

int test_command_bxor_i64(int64_t a, int64_t b) {
    const char buffer[] = {COMMAND_BXOR};
    FILE* fd = fmemopen((void*)buffer, sizeof(buffer), "r");
    enum datum_type types[] = {I64, I64};
    int64_t stack[] = {a, b};

    int32_t* types_ptr = (int32_t*)&types;
    union udatum* udatum_stack = (union udatum*)stack;
    udatum_stack += 1;
    types_ptr += 1;

    povm_execute(fd, udatum_stack, types_ptr);
    munit_assert_int(stack[0], ==, a ^ b);
    fclose(fd);
}

int test_command_add_f64(double a, double b) {
    const char buffer[] = {COMMAND_ADD};
    FILE* fd = fmemopen((void*)buffer, sizeof(buffer), "r");
    enum datum_type types[] = {F64, F64};
    double stack[] = {a, b};

    int32_t* types_ptr = (int32_t*)&types;
    union udatum* udatum_stack = (union udatum*)stack;
    udatum_stack += 1;
    types_ptr += 1;

    povm_execute(fd, udatum_stack, types_ptr);
    munit_assert_double(stack[0], ==, a + b);
    fclose(fd);
}

#include <time.h>
#include <stdlib.h>

static MunitResult test_command_add(const MunitParameter params[], void* user_data) {
    srand(time(NULL));
    test_command_add_i64(rand(), rand());
    test_command_add_f64(-49.0, 44.3);
    return MUNIT_OK;
}

static MunitResult test_command_bxor(const MunitParameter params[], void* user_data) {
    srand(time(NULL));
    test_command_bxor_i64(rand(), rand());
    return MUNIT_OK;
}

static MunitTest test_suite_tests[] = {
  {
    (char*) "test_command_add",
    test_command_add,
    NULL,
    NULL,
    MUNIT_TEST_OPTION_NONE,
    NULL
  },
  {
    (char*) "test_command_bxor (bitwise XOR)",
    test_command_bxor,
    NULL,
    NULL,
    MUNIT_TEST_OPTION_NONE,
    NULL
  },
  { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

static const MunitSuite test_suite = {
  (char*) "",
  test_suite_tests,
  NULL,
  1,
  MUNIT_SUITE_OPTION_NONE
};

int main(int argc, char* argv[MUNIT_ARRAY_PARAM(argc + 1)]) {
  /* Finally, we'll actually run our test suite!  That second argument
   * is the user_data parameter which will be passed either to the
   * test or (if provided) the fixture setup function. */
  return munit_suite_main(&test_suite, (void*) "povm-test", argc, argv);
}

