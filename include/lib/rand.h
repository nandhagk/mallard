#ifndef MALLARD_RAND_H
#define MALLARD_RAND_H 1

#include <cassert>
#include <chrono>
#include <random>

#include "lib/prelude.h"

namespace mld {
static const u64 SEED = std::chrono::steady_clock::now().time_since_epoch().count();
static std::mt19937_64 MT(SEED);
} // namespace mld

#endif // MALLARD_RAND_H
