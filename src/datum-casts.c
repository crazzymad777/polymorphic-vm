#include "datum.h"

#define DATUM_CAST_TO(fn_name, union_member, assign_type) \
struct datum (fn_name)(struct datum d) { \
    int changed = 1; \
    if (d.type == BOOLEAN) { \
        d.union_member = d.boolean; \
    } else if (d.type == I8) { \
        d.union_member = d.i8; \
    } else if (d.type == I16) { \
        d.union_member = d.i16; \
    } else if (d.type == I32) { \
        d.union_member = d.i32; \
    } else if (d.type == I64) { \
        d.union_member = d.i64; \
    } else if (d.type == U8) { \
        d.union_member = d.u8; \
    } else if (d.type == U16) { \
        d.union_member = d.u16; \
    } else if (d.type == U32) { \
        d.union_member = d.u32; \
    } else if (d.type == U64) { \
        d.union_member = d.u64; \
    } else if (d.type == F32) { \
        d.union_member = d.f32; \
    } else if (d.type == F64) { \
        d.union_member = d.f64; \
    } else if (d.type == CODEPOINT) { \
        d.union_member = d.codepoint; \
    } else { \
        changed = 0; \
    } \
    \
    if (changed == 1) { \
        d.type = assign_type; \
    } \
    return d; \
}\


DATUM_CAST_TO(datum_cast_to_f64, f64, F64)
DATUM_CAST_TO(datum_cast_to_f32, f32, F32)

DATUM_CAST_TO(datum_cast_to_u64, u64, U64)
DATUM_CAST_TO(datum_cast_to_u32, u32, U32)
DATUM_CAST_TO(datum_cast_to_u16, u16, U16)
DATUM_CAST_TO(datum_cast_to_u8, u8, U8)

DATUM_CAST_TO(datum_cast_to_i64, i64, I64)
DATUM_CAST_TO(datum_cast_to_i32, i32, I32)
DATUM_CAST_TO(datum_cast_to_i16, i16, I16)
DATUM_CAST_TO(datum_cast_to_i8, i8, I8)

DATUM_CAST_TO(datum_cast_to_bool, boolean, BOOLEAN)
DATUM_CAST_TO(datum_cast_to_char, codepoint, CODEPOINT)

struct datum datum_cast_to_unsigned_integer(struct datum d, int override_size) {
    int sz = datum_sizeof(d);
    if (override_size != -1) {
        sz = override_size;
    }
    if (sz == 4) {
        datum_cast_to_u32(d);
    } else if (sz == 2) {
        datum_cast_to_u16(d);
    } else if (sz == 1) {
        datum_cast_to_u8(d);
    }
    return datum_cast_to_u64(d);
}

struct datum datum_cast_to_signed_integer(struct datum d, int override_size) {
    int sz = datum_sizeof(d);
    if (override_size != -1) {
        sz = override_size;
    }
    if (sz == 4) {
        datum_cast_to_i32(d);
    } else if (sz == 2) {
        datum_cast_to_i16(d);
    } else if (sz == 1) {
        datum_cast_to_i8(d);
    }
    return datum_cast_to_i64(d);
}

