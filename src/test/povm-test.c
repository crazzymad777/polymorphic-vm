#include <povm-gen.h>
#include <munit.h>

int test_command_add_i64(int64_t a, int64_t b);
int test_command_sub_i64(int64_t a, int64_t b);
int test_command_mul_i64(int64_t a, int64_t b);
int test_command_div_i64(int64_t a, int64_t b);
int test_command_rem_i64(int64_t a, int64_t b);
int test_command_add_f64(int64_t a, int64_t b);
int test_command_push_i64(int64_t a);
int test_command_dup_i64(int64_t a);
int test_command_swap_i64(int64_t a, int64_t b);
int test_command_bxor_i64(int64_t a, int64_t b);
int test_command_band_i64(int64_t a, int64_t b);
int test_command_bor_i64(int64_t a, int64_t b);

int test_command_ret();
int test_command_halt_call();
int test_command_halt_jump();
int test_command_jump_to_eof(int64_t x);
void test_command_jump_call();

void test_command_cmp_eq();
void test_command_cmp_not_eq();
void test_command_cmp_gt();
void test_command_cmp_lt();

int test_run_vm_empty_file();
int test_run_vm_wrong_header_signature();
int test_run_vm_simple_program();

#include <time.h>
#include <stdlib.h>

static MunitResult test_command_add(const MunitParameter params[], void* user_data) {
    //srand(time(NULL));
    int a = rand();
    int b = rand();
    test_command_add_i64(a, b);
    test_command_sub_i64(a, b);
    test_command_mul_i64(a, b);
    test_command_div_i64(a, b);
    test_command_rem_i64(a, b);
    test_command_add_f64(-49.0, 44.3);
    return MUNIT_OK;
}

static MunitResult test_command_bxor(const MunitParameter params[], void* user_data) {
    //srand(time(NULL));
    int a = rand();
    int b = rand();
    test_command_bxor_i64(a, b);
    test_command_band_i64(a, b);
    test_command_bor_i64(a, b);
    return MUNIT_OK;
}

static MunitResult test_command_dup(const MunitParameter params[], void* user_data) {
    //srand(time(NULL));
    int a = rand();
    int b = rand();
    test_command_push_i64(a);
    test_command_dup_i64(a);
    test_command_swap_i64(a, b);
    return MUNIT_OK;
}

static MunitResult test_command_call(const MunitParameter params[], void* user_data) {
    //srand(time(NULL));
    test_command_ret();
    test_command_halt_call();
    test_command_halt_jump();
    test_command_jump_to_eof(rand());
    test_command_jump_call();
    return MUNIT_OK;
}

static MunitResult test_cmp(const MunitParameter params[], void* user_data) {
    test_command_cmp_eq();
    test_command_cmp_not_eq();
    test_command_cmp_gt();
    test_command_cmp_lt();
    return MUNIT_OK;
}

static MunitResult test_run_vm(const MunitParameter params[], void* user_data) {
    test_run_vm_empty_file();
    test_run_vm_wrong_header_signature();
    test_run_vm_simple_program();
    return MUNIT_OK;
}

static MunitTest test_suite_tests[] = {
  {
    (char*) "test add/sub/mul/div/rem",
    test_command_add,
    NULL,
    NULL,
    MUNIT_TEST_OPTION_NONE,
    NULL
  },
  {
    (char*) "test bitwise commands",
    test_command_bxor,
    NULL,
    NULL,
    MUNIT_TEST_OPTION_NONE,
    NULL
  },
  {
    (char*) "test push/swap/dup",
    test_command_dup,
    NULL,
    NULL,
    MUNIT_TEST_OPTION_NONE,
    NULL
  },
  {
    (char*) "test ret/call",
    test_command_call,
    NULL,
    NULL,
    MUNIT_TEST_OPTION_NONE,
    NULL
  },
  {
    (char*) "test cmp",
    test_cmp,
    NULL,
    NULL,
    MUNIT_TEST_OPTION_NONE,
    NULL
  },
  {
    (char*) "test run VM",
    test_run_vm,
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

