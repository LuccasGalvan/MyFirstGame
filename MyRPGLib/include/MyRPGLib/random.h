#pragma once

//Randomizer from N to K taken from https://github.com/mattearly/rand
//adapted for unsigned int values

#include <chrono>
#include <random>
#include <cstdint>

#include "types.h"

namespace Random {
    [[nodiscard]] uint32_t NTK(uint32_t n, uint32_t k);
}