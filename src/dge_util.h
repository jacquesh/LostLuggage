#ifndef DGE_UTIL_H
#define DGE_UTIL_H

#include "dge.h"

// Random Number Generation
// From: www.pcg-random.org
// ========================
struct RNGState
{
    uint64 state;
    uint64 inc;
};

uint32 dge_randint(RNGState* rng);
uint32 dge_randint(RNGState* rng, uint32 bound);

float dge_randf(RNGState* rng);
float dge_randf();

void dge_seedRNG(RNGState* rng, uint64 initialState, uint64 initialSequence);

#endif
