#ifndef POVM_LAMBDA_DATUM_H
#define POVM_LAMBDA_DATUM_H

// This file contains only primitive data types

#include <inttypes.h>
#include <stdbool.h>

#if __has_include(<uchar.h>)
	#include <uchar.h>
#else
	typedef uint_least32_t char32_t;
	typedef uint64_t size_t;
	#define MB_LEN_MAX 6
	#define UCHAR_FALLBACK 1
#endif
static size_t codepoint_to_utf8(unsigned char *const buffer, const char32_t code);

#define LAMBDA_COMPARE_BOOLEAN_TO_INTEGER 1
#define LAMBDA_COMPARE_BOOLEAN_TO_FLOATING 1

enum compate_result {
    COMPARE_RESULT_LESS = 1 << 31,
    COMPARE_RESULT_EQUALS = 0,
    COMPARE_RESULT_NOT_EQUALS = 1,
    COMPARE_RESULT_GREATER = 2
};

// List of types
enum datum_type {
VOID,
BOOLEAN,
I8,
I16,
I32,
I64,
U8,
U16,
U32,
U64,
F32,
F64,
CODEPOINT,
COMPARE_RESULT
};

union udatum {
    bool boolean;
    int8_t i8;
    int16_t i16;
    int32_t i32;
    int64_t i64;
    uint8_t u8;
    uint16_t u16;
    uint32_t u32;
    uint64_t u64;
    float f32;
    double f64;
    // const char* cstring;
    char32_t codepoint;
    enum compate_result compare_result;
};

struct datum {
	enum datum_type type;
    union {
        bool boolean;
        int8_t i8;
        int16_t i16;
        int32_t i32;
        int64_t i64;
        uint8_t u8;
        uint16_t u16;
        uint32_t u32;
        uint64_t u64;
        float f32;
        double f64;
        // const char* cstring;
        char32_t codepoint;
        union udatum udatum;
    };
};

#include <result.h>

// Construct Algebraic Data Type (primitive)
struct datum povm_datum(int t, union udatum v);
struct datum datum_void(void);
struct datum datum_bool(bool boolean);

struct datum datum_i8(int8_t integer);
struct datum datum_i16(int16_t integer);
struct datum datum_i32(int32_t integer);
struct datum datum_i64(int64_t integer);

struct datum datum_u8(uint8_t unsigned_integer);
struct datum datum_u16(uint16_t unsigned_integer);
struct datum datum_u32(uint32_t unsigned_integer);
struct datum datum_u64(uint64_t unsigned_integer);

struct datum datum_f32(float floating);
struct datum datum_f64(double floating);
struct datum datum_codepoint(char32_t codepoint);

// Info about type
bool datum_is_void(struct datum datum);
bool datum_is_logical(struct datum datum);
bool datum_is_numeric(struct datum datum);
bool datum_is_integer(struct datum datum);
bool datum_is_floating(struct datum datum);
size_t datum_sizeof(struct datum datum);
size_t datum_type_sizeof(enum datum_type type);
bool datum_is_signed_integer(struct datum datum);
bool datum_is_unsigned_integer(struct datum datum);
bool datum_is_charcode(struct datum datum);

// Relations operators
bool datum_op_equals(struct datum p0, struct datum p1);
bool datum_op_not_equals(struct datum p0, struct datum p1);
bool datum_op_gt(struct datum p0, struct datum p1);
bool datum_op_gte(struct datum p0, struct datum p1); // great than or equals
bool datum_op_lt(struct datum p0, struct datum p1);
bool datum_op_lte(struct datum p0, struct datum p1); // less than or equals

// Arithmetical operators
struct result datum_add(struct datum p0, struct datum p1);
struct result datum_sub(struct datum p0, struct datum p1);
struct result datum_mul(struct datum p0, struct datum p1);
struct result datum_div(struct datum p0, struct datum p1);
struct result datum_rem(struct datum p0, struct datum p1); // %

// Logical operators
struct result datum_logical_not(struct datum p0);
struct result datum_logical_and(struct datum p0, struct datum p1);
struct result datum_logical_or(struct datum p0, struct datum p1);

// Bitwise operators
struct result datum_bitwise_xor(struct datum p0, struct datum p1);
struct result datum_bitwise_and(struct datum p0, struct datum p1);
struct result datum_bitwise_or(struct datum p0, struct datum p1);
struct result datum_right_shift(struct datum p0, int bits);
struct result datum_left_shift(struct datum p0, int bits);

// Casting
struct datum datum_cast_to_f64(struct datum p0);
struct datum datum_cast_to_f32(struct datum p0);
struct datum datum_cast_to_u64(struct datum p0);
struct datum datum_cast_to_u32(struct datum p0);
struct datum datum_cast_to_u16(struct datum p0);
struct datum datum_cast_to_u8(struct datum p0);
struct datum datum_cast_to_i64(struct datum p0);
struct datum datum_cast_to_i32(struct datum p0);
struct datum datum_cast_to_i16(struct datum p0);
struct datum datum_cast_to_i8(struct datum p0);
struct datum datum_cast_to_char(struct datum p0);
struct datum datum_cast_to_bool(struct datum p0);

struct datum datum_cast_to_unsigned_integer(struct datum d, int override_size);
struct datum datum_cast_to_signed_integer(struct datum d, int override_size);

// Auxiliary functions
const char* datum_get_type(enum datum_type datum);
void datum_to_string(struct datum datum, char* buffer, int siz);
struct datum datum_upgrade_precision(struct datum datum);

#endif

