#include "types.h"

static uint32 seed = 1;

static const uint64 MODULUS = 2147483648;
static const uint64 MULTIPLIER = 1103515245;
static const uint64 INCREMENT = 12345;

static int
rand(void) {
  seed = (MULTIPLIER * seed + INCREMENT) % MODULUS;
  return seed;
}