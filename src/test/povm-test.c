#include <povm-gen.h>
#include <munit.h>

int test_command_add_i64(int64_t a, int64_t b);
int test_command_add_f64(int64_t a, int64_t b);
int test_command_dup_i64(int64_t a);
int test_command_bxor_i64(int64_t a, int64_t b);
int test_command_band_i64(int64_t a, int64_t b);
int test_command_bor_i64(int64_t a, int64_t b);

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
    int a = rand();
    int b = rand();
    test_command_bxor_i64(a, b);
    test_command_band_i64(a, b);
    test_command_bor_i64(a, b);
    return MUNIT_OK;
}

static MunitResult test_command_dup(const MunitParameter params[], void* user_data) {
    srand(time(NULL));
    test_command_dup_i64(rand());
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
    (char*) "test_command_bxor (bitwise commands)",
    test_command_bxor,
    NULL,
    NULL,
    MUNIT_TEST_OPTION_NONE,
    NULL
  },
  {
    (char*) "test_command_dup (duplicate)",
    test_command_dup,
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

