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

int test_run_vm_simple_program() {
    const char buffer[114] = "\x50\x52\x4F\x54\x45\x43\x54\x32\x00\x00\x00\x00\x00\x00\x00\x00\x02\x05\x00\x00\x00\x02\x00\x00\x00\x00\x00\x00\x00\x05\x02\x05\x00\x00\x00\x02\x00\x00\x00\x00\x00\x00\x00\x07\x18\x02\x05\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x1C\x03\x05\x04\x18\x02\x05\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x1C\x03\x03\x03\x18\x02\x05\x00\x00\x00\x02\x00\x00\x00\x00\x00\x00\x00\x1C\x03\x05\x09\x18\x02\x05\x00\x00\x00\x01\x00\x00\x00\x00\x00\x00\x00\x1C\x03";

    char out_buffer[5] = {0, 0, 0, 0, 0};

    FILE* fd = fmemopen((void*)buffer, sizeof(buffer), "r");
    FILE* fd_vm_out = fmemopen((void*)out_buffer, sizeof(out_buffer), "w");
    struct povm_io_streams streams = {
        stdin,
        fd_vm_out,
        stderr
    };

    munit_assert_int(run_vm_custom_streams(fd, 1, streams), ==, 0);
    munit_assert_string_equal(out_buffer, "0021");
}
