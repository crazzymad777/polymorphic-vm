#include <povm-gen.h>
#include <run-vm.h>
#include <munit.h>

int test_run_vm_empty_file() {
    FILE* fd = fmemopen(NULL, 0, "r");
    munit_assert_int(run_vm(fd, 1), ==, -1);
}

int test_run_vm_wrong_header_signature() {
    const char buffer[] = "12345678";
    FILE* fd = fmemopen((void*)buffer, sizeof(buffer), "r");
    munit_assert_int(run_vm(fd, 1), ==, -2);
}
