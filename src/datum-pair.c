#include "datum-pair.h"

#define PAIR_MAJOR_UNDEFINED -1
#define PAIR_MAJOR_FLOATED_FIRST 0
#define PAIR_MAJOR_FLOATED_SECOND 1
#define PAIR_MAJOR_UNSIGNED_FIRST 2
#define PAIR_MAJOR_UNSIGNED_SECOND 3
#define PAIR_MAJOR_SIGNED_FIRST 4
#define PAIR_MAJOR_SIGNED_SECOND 5
#define PAIR_MAJOR_FIRST 6
#define PAIR_MAJOR_SECOND 7

// matrix of common type
//      void f64 f32 u64 u32 u16 u8  i64 i32 i16 i8 char bool
// void void f64 f32 u64 u32 u16 u8  i64 i32 i16 i8 char bool
// f64    -  f64 f64 f64 f64 f64 f64 f64 f64 f64 f64 f64  f64
// f32    -  -   f32 f32 f32 f32 f32 f32 f32 f32 f32 f32  f32
// u64    -  -   -   u64 u64 u64 u64 u64 u64 u64 u64 u64  u64
// u32    -  -   -   -   u32 u32 u32 u64 u32 u32 u32 u32  u32
// u16    -  -   -   -   -   u16 u16 u64 u32 u16 u16 u32  u16
// u8     -  -   -   -   -   -   u8  u64 u32 u16 u8  u32   u8
// i64    -  -   -   -   -   -   -   i64 i64 i64 i64 i64  i64
// i32    -  -   -   -   -   -   -   -   i32 i32 i32 i32  i32
// i16    -  -   -   -   -   -   -   -   -   i16 i16 i32  i16
// i8     -  -   -   -   -   -   -   -   -   -   i8  i32  i8
// char   -  -   -   -   -   -   -   -   -   -   -   char char
// bool   -  -   -   -   -   -   -   -   -   -   -   -    bool

// 1) "precision" never decrease (you can't lost data)
// 2) unsigned integer never became signed (troubles with sign required to be solved)

// Two steps:
// 1) Determine more common operand
// 2) Cast less common operand to more common operand

// And we MUST preserve operand order

struct datum_pair make_pair(struct datum p0, struct datum p1) {
    struct datum_pair pair;
    pair.first = p0;
    pair.second = p1;
    return pair;
}

struct datum_pair datum_pair_commonize(struct datum_pair pair) {
    if (datum_is_void(pair.first) || datum_is_void(pair.second)) {
        return pair;
    }

    if (pair.first.type == F64) {
        pair.second = datum_cast_to_f64(pair.second);
        return pair;
    }

    if (pair.second.type == F64) {
        pair.first = datum_cast_to_f64(pair.first);
        return pair;
    }

    if (pair.first.type == F32) {
        pair.second = datum_cast_to_f32(pair.second);
        return pair;
    }

    if (pair.second.type == F32) {
        pair.first = datum_cast_to_f32(pair.first);
        return pair;
    }

    int unsign = 0;
    if (datum_is_unsigned_integer(pair.first) || datum_is_unsigned_integer(pair.second)) {
        unsign = 1;
    }

    int siz = datum_sizeof(pair.first);
    int siz2 = datum_sizeof(pair.second);
    if (siz2 > siz2) {
        siz = siz2;
    }

    if (unsign) {
        pair.first = datum_cast_to_unsigned_integer(pair.first, siz);
        pair.second = datum_cast_to_unsigned_integer(pair.second, siz);
    } else {
        pair.first = datum_cast_to_signed_integer(pair.first, siz);
        pair.second = datum_cast_to_signed_integer(pair.second, siz);
    }

    return pair;
}
