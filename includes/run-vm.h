#ifndef POVM_RUN_VM_H
#define POVM_RUN_VM_H

#include <povm.h>

int run_vm(FILE *fd, int close_fd);
int run_vm_custom_streams(FILE* fd, int close_fd, struct povm_io_streams streams);

#endif
