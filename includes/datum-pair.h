#ifndef LAMBDA_DATUM_PAIR_H
#define LAMBDA_DATUM_PAIR_H

#include "datum.h"

struct datum_pair {
    struct datum first;
    struct datum second;
};

struct datum_pair datum_pair_commonize(struct datum_pair pair);
struct datum_pair make_pair(struct datum p0, struct datum p1);

#endif
