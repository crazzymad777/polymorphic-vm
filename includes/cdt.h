#ifndef POVM_CDT_H
#define POVM_CDT_H


// plain stack, autogrow stack...
// vector
// objects, maps, interfaces, functions
enum cdt_type {
  POVM_STACK
};

struct cdt {
    int counter;
    enum cdt_type type;
    void* pointer;
    void* pointer_types;
    int droppable;
    // handle opcode...
};

void cdt_release(struct cdt* cdt) {
    cdt->counter--;
    if (cdt->counter <= 0 && cdt->droppable) {
        free(cdt->pointer);
        free(cdt->pointer_types);
    }
}

void cdt_use(struct cdt* cdt) {
    cdt->counter++;
}

struct cdt cdt_create_povm_stack() {
    struct cdt cdt = {1, POVM_STACK, malloc(4096), malloc(4096 / 2), 1};
    return cdt;
}

#endif
