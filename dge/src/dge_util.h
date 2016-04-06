#ifndef DGE_UTIL_H
#define DGE_UTIL_H

#include "dge.h"

namespace dge
{
    // Random Number Generation
    // From: www.pcg-random.org
    // ========================
    struct RNGState
    {
        uint64 state;
        uint64 inc;
    };

    uint32 randint(RNGState* rng);
    uint32 randint(RNGState* rng, uint32 bound);

    float randf(RNGState* rng);
    float randf();

    void seedRNG(RNGState* rng, uint64 initialState, uint64 initialSequence);
}

#endif
