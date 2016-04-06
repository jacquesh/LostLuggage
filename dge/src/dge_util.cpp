#include "dge_util.h"

// Returns a random, uniformly distributed, unsigned 32-bit integer
uint32 dge::randint(dge::RNGState* rng)
{
    uint64 oldstate = rng->state;
    rng->state = oldstate * 6364136223846793005ULL + (rng->inc|1);
    uint32 xorshifted = ((oldstate >> 18u) ^ oldstate) >> 27u;
    uint32 rot = oldstate >> 59u;
    return (xorshifted >> rot) | (xorshifted << ((-rot) & 31));
}

// Returns a random, uniformly distributed, unsigned integer r in the range 0 <= r < bound
uint32 dge::randint(dge::RNGState* rng, uint32 bound)
{
    uint32 threshold = -bound % bound;
    for (;;) {
        uint32 r = dge::randint(rng);
        if (r >= threshold)
        {
            return r % bound;
        }
    }
}

// Returns a random, uniformly distributed 32-bit float r in the range 0.0 <= r < 1.0
float dge::randf(dge::RNGState* rng)
{
    uint32 randint = dge::randint(rng);
    float result = randint * (1.0f/(float)UINT32_MAX);

    return result;
}

// TODO: Remove this, this is just a convenience function that doesn't require you to pass in
//       the RNGState each time
float dge::randf()
{
    // NOTE: This is just the initial state given by the example code for their static initializer,
    //       In some sense it's useful because it gives us some consistency but ultimately we
    //       don't want to use this function at all
    static dge::RNGState rng = { 0x853c49e6748fea9bULL, 0xda3e39cb94b95bdbULL };
    float result = dge::randf(&rng);
    return result;
}

// Seeds the given RNGState using the initialState and initialSequence parameters
void dge::seedRNG(dge::RNGState* rng, uint64 initialState, uint64 initialSequence)
{
    rng->state = 0u;
    rng->inc = (initialSequence << 1u) | 1u;
    dge::randint(rng);
    rng->state += initialState;
    dge::randint(rng);
}
