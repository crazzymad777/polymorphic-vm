#include "datum.h"
#include "string.h"
#include "datum_compare.h"

//enum OP_EQUALS_ENUM {
//    PASS,
//    RETURN_FALSE,
//    RETURN_TRUE
//};

bool datum_op_equals(struct datum p0, struct datum p1) {
    if (datum_is_void(p0) || datum_is_void(p1)) {
         return datum_op_compare_void(p0, p1) == OP_COMPARE_EQUALS ? true : false;
    }

    if (datum_is_logical(p0) || datum_is_logical(p1)) {
        return datum_op_equals_boolean(p0, p1);
    }

    /*if (datum_is_string(p0) || datum_is_string(p1)) {
        return datum_op_compare_string(p0, p1);
    }*/

    if (p0.type == CODEPOINT) {
        if (p1.type == CODEPOINT) {
            return p0.codepoint == p1.codepoint;
        }
    }

    struct datum p0new = datum_upgrade_precision(p0);
    struct datum p1new = datum_upgrade_precision(p1);

    if (datum_is_signed_integer(p0new)) {
        if (p1new.type == CODEPOINT) {
            return p0new.i64 == p1new.codepoint;
        }

        if (datum_is_signed_integer(p1new)) {
            return p0new.i64 == p1new.i64;
        }
        if (datum_is_unsigned_integer(p1new)) {
            //if (p0new.i64 < 0) {
            //    return false;
            //}
            return p0new.i64 == p1new.u64;
        }
        if (datum_is_floating(p1new)) {
            return p0new.i64 == p1new.f64;
        }
    }

    if (datum_is_unsigned_integer(p0new)) {
        if (p1new.type == CODEPOINT) {
            return p0new.u64 == p1new.codepoint;
        }

        if (datum_is_signed_integer(p1new)) {
            return p0new.u64 == p1new.i64;
        }
        if (datum_is_unsigned_integer(p1new)) {
            return p0new.u64 == p1new.u64;
        }
        if (datum_is_floating(p1new)) {
            return p0new.u64 == p1new.f64;
        }
    }

    if (datum_is_floating(p0new)) {
        if (p1new.type == CODEPOINT) {
            return p0new.f64 == p1new.codepoint;
        }

        if (datum_is_signed_integer(p1new)) {
            return p0new.f64 == p1new.i64;
        }
        if (datum_is_unsigned_integer(p1new)) {
            return p0new.f64 == p1new.u64;
        }
        if (datum_is_floating(p1new)) {
            return p0new.f64 == p1new.f64;
        }
    }

    return false;
}

bool datum_op_not_equals(struct datum p0, struct datum p1) {
    return !datum_op_equals(p0, p1);
}
