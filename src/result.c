#include "result.h"

struct result result_from_error_code(long int error_code) {
    struct result result = {ERROR, .error_code = error_code};
    return result;
}

struct result result_from_datum(struct datum datum) {
    struct result result = {RESULT, .datum = datum};
    return result;
}
