#include "datum_compare.h"
#include <stdbool.h>

// If at least one of operands is VOID call this function
enum OP_COMPARE datum_op_compare_void(struct datum p0, struct datum p1) {
    return (datum_is_void(p0) && datum_is_void(p1)) ? OP_COMPARE_EQUALS : OP_COMPARE_NOT_EQUALS;
}

// If at least one of operands is BOOLEAN call this function
bool datum_op_equals_boolean(struct datum p0, struct datum p1) {
    if (datum_is_logical(p0) && datum_is_logical(p1)) {
        return p0.boolean == p1.boolean;
    }


    // SIZES!!!
#ifdef LAMBDA_COMPARE_BOOLEAN_TO_INTEGER
    // Integer?
    if (datum_is_integer(p0)) {
        return p0.i64 == p1.boolean;
    }

    if (datum_is_integer(p1)) {
        return p1.i64 == p0.boolean;
    }
#endif
    // floating?
    // Probably not
    // but if 1 == true then 1.0f == true
#ifdef LAMBDA_COMPARE_BOOLEAN_TO_FLOATING
    if (datum_is_floating(p0)) {
        struct datum p0new = datum_upgrade_precision(p0);
        return p0new.f64 == p1.boolean;
    }

    if (datum_is_floating(p1)) {
        struct datum p1new = datum_upgrade_precision(p1);
        return p1new.f64 == p0.boolean;
    }
#endif

    return false;
}

enum OP_COMPARE datum_op_compare_boolean(struct datum p0, struct datum p1) {
    return datum_op_equals_boolean(p0, p1) ? OP_COMPARE_EQUALS : OP_COMPARE_NOT_EQUALS;
}
