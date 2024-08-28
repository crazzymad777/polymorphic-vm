#include "datum.h"
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#include "datum-pair.h"

struct datum povm_datum(int t, union udatum v) {
    struct datum datum = {t, .udatum = v};
    return datum;
}

struct datum povm_datum_by_i64(int t, int64_t v) {
    struct datum datum = {t, .i64 = v};
    return datum;
}

struct datum datum_void(void) {
    struct datum datum = {VOID};
    return datum;
}

struct datum datum_bool(bool boolean) {
    struct datum datum = {BOOLEAN, .boolean = boolean};
    return datum;
}

struct datum datum_i8(int8_t integer) {
    struct datum datum = {I8, .i8 = integer};
    return datum;
}
struct datum datum_i16(int16_t integer) {
    struct datum datum = {I16, .i16 = integer};
    return datum;
}
struct datum datum_i32(int32_t integer) {
    struct datum datum = {I32, .i32 = integer};
    return datum;
}
struct datum datum_i64(int64_t integer) {
    struct datum datum = {I64, .i64 = integer};
    return datum;
}


struct datum datum_u8(uint8_t unsigned_integer) {
    struct datum datum = {U8, .u8 = unsigned_integer};
    return datum;
}
struct datum datum_u16(uint16_t unsigned_integer) {
    struct datum datum = {U16, .u16 = unsigned_integer};
    return datum;
}
struct datum datum_u32(uint32_t unsigned_integer) {
    struct datum datum = {U32, .u32 = unsigned_integer};
    return datum;
}
struct datum datum_u64(uint64_t unsigned_integer) {
    struct datum datum = {U64, .u64 = unsigned_integer};
    return datum;
}

struct datum datum_f32(float floating) {
    struct datum datum = {F32, .f32 = floating};
    return datum;
}
struct datum datum_f64(double floating) {
    struct datum datum = {F64, .f64 = floating};
    return datum;
}

struct datum datum_codepoint(char32_t codepoint) {
    struct datum datum = {CODEPOINT, .codepoint = codepoint};
    return datum;
}

bool datum_is_void(struct datum datum) {
    return datum.type == VOID;
}

bool datum_is_logical(struct datum datum) {
    return datum.type == BOOLEAN;
}

bool datum_is_numeric(struct datum datum) {
    return datum_is_integer(datum) || datum_is_floating(datum);
}

bool datum_is_integer(struct datum datum) {
    return datum_is_unsigned_integer(datum) || datum_is_signed_integer(datum);
}

bool datum_is_floating(struct datum datum) {
    return datum.type == F32 || datum.type == F64;
}

size_t datum_sizeof(struct datum datum) {
    return datum_type_sizeof(datum.type);
}

size_t datum_type_sizeof(enum datum_type type) {
    if (type == VOID) return 0;
    if (type == BOOLEAN) return sizeof(bool);
    if (type == I8 || type == U8) return 1;
    if (type == I16 || type == U16) return 2;
    if (type == I32 || type == U32 || type == F32 || type == CODEPOINT) return 4;
    if (type == I64 || type == U64 || type == F64) return 8;
    return -1;
}

bool datum_is_signed_integer(struct datum datum) {
    return datum.type == I8 || datum.type == I16 || datum.type == I32 || datum.type == I64;
}

bool datum_is_unsigned_integer(struct datum datum) {
    return datum.type == U8 || datum.type == U16 || datum.type == U32 || datum.type == U64;
}

bool datum_is_charcode(struct datum datum) {
    return datum.type == CODEPOINT;
}

bool datum_implements_add(struct datum datum) {
    if (datum_is_numeric(datum) || datum.type == CODEPOINT) return true;
    return false;
}

bool datum_implements_sub(struct datum datum) {
    if (datum_is_numeric(datum) || datum.type == CODEPOINT) return true;
    return false;
}

bool datum_implements_mul(struct datum datum) {
    if (datum_is_numeric(datum)) return true;
    return false;
}
bool datum_implements_div(struct datum datum) {
    if (datum_is_numeric(datum)) return true;
    return false;
}

bool datum_implements_logical_ops(struct datum datum) {
    if (datum_is_logical(datum)) {
        return true;
    }
    return false;
} // !, &&, ||
bool datum_implements_related_ops(struct datum datum) {
    if (datum_is_numeric(datum) || datum.type == CODEPOINT) {
        return true;
    }
    return false;
} // >, >=, <, <=

bool datum_implements_bitwise_ops(struct datum datum) {
    if (datum_is_integer(datum)) {
        return true;
    }
    return false;
}

__attribute__((visibility ("hidden")))
struct datum datum_upgrade_precision(struct datum datum) {
    if (datum_is_logical(datum)) {
        return datum_i64(datum.boolean);
    }

    if (datum_is_signed_integer(datum)) {
        return datum_cast_to_i64(datum);
    }
    if (datum_is_unsigned_integer(datum)) {
        return datum_cast_to_u64(datum);
    }
    if (datum_is_floating(datum)) {
        return datum_cast_to_f64(datum);
    }
    if (datum.type == CODEPOINT) {
        return datum;
    }
    return datum_void();
}

/* const char* datum_get_type(struct datum datum) {
    static const char* types_str[] = {
        [VOID] = "void",
        [BOOLEAN] = "bool",
        [I8] = "i8",
        [I16] = "i16",
        [I32] = "i32",
        [I64] = "i64",
        [U8] = "u8",
        [U16] = "u16",
        [U32] = "u32",
        [U64] = "u64",
        [F32] = "f32",
        [F64] = "f64",
        [CODEPOINT] = "char"
    };
    return types_str[datum.type];
} */

const char* datum_get_type(enum datum_type type) {
    static const char* types_str[] = {
        [VOID] = "void",
        [BOOLEAN] = "bool",
        [I8] = "i8",
        [I16] = "i16",
        [I32] = "i32",
        [I64] = "i64",
        [U8] = "u8",
        [U16] = "u16",
        [U32] = "u32",
        [U64] = "u64",
        [F32] = "f32",
        [F64] = "f64",
        [CODEPOINT] = "char"
    };
    return types_str[type];
}

__attribute__((visibility ("hidden")))
size_t codepoint_to_utf8(unsigned char *const buffer, const char32_t code)
{
    if (code <= 0x7F) {
        buffer[0] = code;
        return 1;
    }
    if (code <= 0x7FF) {
        buffer[0] = 0xC0 | (code >> 6);            /* 110xxxxx */
        buffer[1] = 0x80 | (code & 0x3F);          /* 10xxxxxx */
        return 2;
    }
    if (code <= 0xFFFF) {
        buffer[0] = 0xE0 | (code >> 12);           /* 1110xxxx */
        buffer[1] = 0x80 | ((code >> 6) & 0x3F);   /* 10xxxxxx */
        buffer[2] = 0x80 | (code & 0x3F);          /* 10xxxxxx */
        return 3;
    }
    if (code <= 0x10FFFF) {
        buffer[0] = 0xF0 | (code >> 18);           /* 11110xxx */
        buffer[1] = 0x80 | ((code >> 12) & 0x3F);  /* 10xxxxxx */
        buffer[2] = 0x80 | ((code >> 6) & 0x3F);   /* 10xxxxxx */
        buffer[3] = 0x80 | (code & 0x3F);          /* 10xxxxxx */
        return 4;
    }
    return 0;
}

// add function with literal form??
void datum_to_string(struct datum datum, char* buffer, int siz) {
    if (datum.type == VOID) {
        snprintf(buffer, siz, "(null)");
    }
    else if (datum.type == BOOLEAN) {
        snprintf(buffer, siz, "%s", datum.boolean ? "true" : "false");
    }
    else if (datum.type == I8) {snprintf(buffer, siz, "%d", datum.i8); return;}
    else if (datum.type == I16) {snprintf(buffer, siz, "%d", datum.i16); return;}
    else if (datum.type == I32) {snprintf(buffer, siz, "%d", datum.i32); return;}
    else if (datum.type == I64) {snprintf(buffer, siz, "%d", datum.i64); return;}
    else if (datum.type == U8) {snprintf(buffer, siz, "%d", datum.u8); return;}
    else if (datum.type == U16) {snprintf(buffer, siz, "%d", datum.u16); return;}
    else if (datum.type == U32) {snprintf(buffer, siz, "%d", datum.u32); return;}
    else if (datum.type == U64) {snprintf(buffer, siz, "%d", datum.u64); return;}
    else if (datum.type == F32) {snprintf(buffer, siz, "%f", datum.f32); return;}
    else if (datum.type == F64) {snprintf(buffer, siz, "%f", datum.f64); return;}
    else if (datum.type == CODEPOINT) {
        char buf[MB_LEN_MAX + 1] = {0};
		int sz = codepoint_to_utf8(buf, datum.codepoint);
		buf[sz] = '\0';
        snprintf(buffer, siz, "%s", buf);
    } else {
        snprintf(buffer, siz, "");
    }
}

bool datum_op_gt(struct datum p0, struct datum p1) {
    if (p0.type == CODEPOINT && p1.type == CODEPOINT) {
        return p0.codepoint > p1.codepoint;
    }
    struct datum p0new = datum_upgrade_precision(p0);
    struct datum p1new = datum_upgrade_precision(p1);

    if (datum_is_signed_integer(p0new)) {
        //CODEPOINT!!!! HERE INSERT
        if (p1new.type == CODEPOINT) {
            return p0new.i64 > p1new.codepoint;
        }

        if (datum_is_signed_integer(p1new)) {
            return p0new.i64 > p1new.i64;
        }
        if (datum_is_unsigned_integer(p1new)) {
            //if (p0new.i64 < 0) {
            //    return false;
            //}
            return p0new.i64 > p1new.u64;
        }
        if (datum_is_floating(p1new)) {
            return p0new.i64 > p1new.f64;
        }
    }

    if (datum_is_unsigned_integer(p0new)) {
        //CODEPOINT!!!! HERE INSERT
        if (p1new.type == CODEPOINT) {
            return p0new.u64 > p1new.codepoint;
        }

        if (datum_is_signed_integer(p1new)) {
            return p0new.u64 > p1new.i64;
        }
        if (datum_is_unsigned_integer(p1new)) {
            return p0new.u64 > p1new.u64;
        }
        if (datum_is_floating(p1new)) {
            return p0new.u64 > p1new.f64;
        }
    }

    if (datum_is_floating(p0new)) {
        //CODEPOINT!!!! HERE INSERT

        if (datum_is_signed_integer(p1new)) {
            return p0new.f64 > p1new.i64;
        }
        if (datum_is_unsigned_integer(p1new)) {
            return p0new.f64 > p1new.u64;
        }
        if (datum_is_floating(p1new)) {
            return p0new.f64 > p1new.f64;
        }
    }
    return false;
}

bool datum_op_gte(struct datum p0, struct datum p1) {
    if(datum_op_equals(p0, p1)) return true;
    return datum_op_gt(p0, p1);
} // great than or equals

bool datum_op_lt(struct datum p0, struct datum p1) {
    if (p0.type == CODEPOINT && p1.type == CODEPOINT) {
        return p0.codepoint < p1.codepoint;
    }
    struct datum p0new = datum_upgrade_precision(p0);
    struct datum p1new = datum_upgrade_precision(p1);

    if (datum_is_signed_integer(p0new)) {
        //CODEPOINT!!!! HERE INSERT
        if (p1new.type == CODEPOINT) {
            return p0new.i64 < p1new.codepoint;
        }

        if (datum_is_signed_integer(p1new)) {
            return p0new.i64 < p1new.i64;
        }
        if (datum_is_unsigned_integer(p1new)) {
            //if (p0new.i64 < 0) {
            //    return false;
            //}
            return p0new.i64 < p1new.u64;
        }
        if (datum_is_floating(p1new)) {
            return p0new.i64 < p1new.f64;
        }
    }

    if (datum_is_unsigned_integer(p0new)) {
        //CODEPOINT!!!! HERE INSERT
        if (p1new.type == CODEPOINT) {
            return p0new.u64 < p1new.codepoint;
        }

        if (datum_is_signed_integer(p1new)) {
            return p0new.u64 < p1new.i64;
        }
        if (datum_is_unsigned_integer(p1new)) {
            return p0new.u64 < p1new.u64;
        }
        if (datum_is_floating(p1new)) {
            return p0new.u64 < p1new.f64;
        }
    }

    if (datum_is_floating(p0new)) {
        //CODEPOINT!!!! HERE INSERT
        if (p1new.type == CODEPOINT) {
            return p0new.f64 < p1new.codepoint;
        }

        if (datum_is_signed_integer(p1new)) {
            return p0new.f64 < p1new.i64;
        }
        if (datum_is_unsigned_integer(p1new)) {
            return p0new.f64 < p1new.u64;
        }
        if (datum_is_floating(p1new)) {
            return p0new.f64 < p1new.f64;
        }
    }
    return false;
}

bool datum_op_lte(struct datum p0, struct datum p1) {
    if(datum_op_equals(p0, p1)) return true;
    return datum_op_lt(p0, p1);
} // lest than or equals

struct result datum_bitwise_xor(struct datum p0, struct datum p1) {
    if (datum_is_floating(p0) || datum_is_floating(p1)) {
        return result_from_error_code(LAMBDA_INVALID_OPERANDS);
    }
    struct datum r0;
    r0.type = p0.type;
    r0.i64 = p0.i64 ^ p1.i64;
    return result_from_datum(r0);
}

struct result datum_bitwise_and(struct datum p0, struct datum p1) {
    if (datum_is_floating(p0) || datum_is_floating(p1)) {
        return result_from_error_code(LAMBDA_INVALID_OPERANDS);
    }

    struct datum r0;
    r0.type = p0.type;
    r0.i64 = p0.i64 & p1.i64;
    return result_from_datum(r0);
}

struct result datum_bitwise_or(struct datum p0, struct datum p1) {
    if (datum_is_floating(p0) || datum_is_floating(p1)) {
        return result_from_error_code(LAMBDA_INVALID_OPERANDS);
    }

    struct datum r0;
    r0.type = p0.type;
    r0.i64 = p0.i64 | p1.i64;
    return result_from_datum(r0);
}

struct result datum_right_shift(struct datum p0, int bits) {
    if (datum_is_floating(p0)) {
        return result_from_error_code(LAMBDA_INVALID_OPERANDS);
    }

    struct datum r0;
    r0.type = p0.type;
    r0.i64 = p0.i64 >> bits;
    return result_from_datum(r0);
}

struct result datum_left_shift(struct datum p0, int bits) {
    if (datum_is_floating(p0)) {
        return result_from_error_code(LAMBDA_INVALID_OPERANDS);
    }

    struct datum r0;
    r0.type = p0.type;
    r0.i64 = p0.i64 << bits;
    return result_from_datum(r0);
}

