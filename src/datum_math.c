#include "datum-pair.h"

#define LAMBDA_MATH_OPERATOR(fn_name, op) \
struct result (fn_name)(struct datum p0, struct datum p1) {\
    if (datum_is_void(p0) || datum_is_void(p1)) {\
        return result_from_datum(datum_void());\
    }\
    struct datum_pair pair = make_pair(p0, p1);\
    pair = datum_pair_commonize(pair);\
    p0 = pair.first;\
    p1 = pair.second;\
\
    if (datum_is_floating(p0)) {\
        if (datum_sizeof(p0) == 8) {\
            return result_from_datum(datum_f64(p0.f64 op p1.f64));\
        } else {\
            return result_from_datum(datum_f32(p0.f32 op p1.f32));\
        }\
    }\
    if (datum_is_unsigned_integer(p0)) {\
        if (datum_sizeof(p0) == 8) {\
            return result_from_datum(datum_u64(p0.u64 op p1.u64));\
        } else if (datum_sizeof(p0) == 4) {\
            return result_from_datum(datum_u32(p0.u32 op p1.u32));\
        } else if (datum_sizeof(p0) == 2) {\
            return result_from_datum(datum_u16(p0.u16 op p1.u16));\
        } else if (datum_sizeof(p0) == 1) {\
            return result_from_datum(datum_u8(p0.u8 op p1.u8));\
        }\
    }\
    if (datum_is_signed_integer(p0)) {\
        if (datum_sizeof(p0) == 8) {\
            return result_from_datum(datum_i64(p0.i64 op p1.i64));\
        } else if (datum_sizeof(p0) == 4) {\
            return result_from_datum(datum_i32(p0.i32 op p1.i32));\
        } else if (datum_sizeof(p0) == 2) {\
            return result_from_datum(datum_i16(p0.i16 op p1.i16));\
        } else if (datum_sizeof(p0) == 1) {\
            return result_from_datum(datum_u8(p0.i8 op p1.i8));\
        }\
    }\
    if (datum_is_charcode(p0)) {\
        return result_from_datum(datum_codepoint(p0.codepoint op p1.codepoint));\
    }\
    if (datum_is_logical(p0)) {\
        return result_from_datum(datum_bool(p0.boolean op p1.boolean));\
    }\
    return result_from_error_code(LAMBDA_UNDEFINED_BEHAVIOR);\
}\

LAMBDA_MATH_OPERATOR(datum_add, +)
LAMBDA_MATH_OPERATOR(datum_sub, -)
LAMBDA_MATH_OPERATOR(datum_mul, *)
LAMBDA_MATH_OPERATOR(datum_div, /)

struct result datum_rem(struct datum p0, struct datum p1) {
    if (datum_is_floating(p0) || datum_is_floating(p1)) {
        return result_from_error_code(LAMBDA_INVALID_OPERANDS);
    }

    if (datum_is_void(p0) || datum_is_void(p1)) {
        return result_from_datum(datum_void());
    }
    struct datum_pair pair = make_pair(p0, p1);
    pair = datum_pair_commonize(pair);
    p0 = pair.first;
    p1 = pair.second;

    if (datum_is_unsigned_integer(p0)) {
        if (datum_sizeof(p0) == 8) {
            return result_from_datum(datum_u64(p0.u64 % p1.u64));
        } else if (datum_sizeof(p0) == 4) {
            return result_from_datum(datum_u32(p0.u32 % p1.u32));
        } else if (datum_sizeof(p0) == 2) {
            return result_from_datum(datum_u16(p0.u16 % p1.u16));
        } else if (datum_sizeof(p0) == 1) {
            return result_from_datum(datum_u8(p0.u8 % p1.u8));
        }
    }
    if (datum_is_signed_integer(p0)) {
        if (datum_sizeof(p0) == 8) {
            return result_from_datum(datum_i64(p0.i64 % p1.i64));
        } else if (datum_sizeof(p0) == 4) {
            return result_from_datum(datum_i32(p0.i32 % p1.i32));
        } else if (datum_sizeof(p0) == 2) {
            return result_from_datum(datum_i16(p0.i16 % p1.i16));
        } else if (datum_sizeof(p0) == 1) {
            return result_from_datum(datum_u8(p0.i8 % p1.i8));
        }
    }
    if (datum_is_charcode(p0)) {
        return result_from_datum(datum_codepoint(p0.codepoint % p1.codepoint));
    }
    if (datum_is_logical(p0)) {
        return result_from_datum(datum_bool(p0.boolean % p1.boolean));
    }
    return result_from_error_code(LAMBDA_UNDEFINED_BEHAVIOR);
}

