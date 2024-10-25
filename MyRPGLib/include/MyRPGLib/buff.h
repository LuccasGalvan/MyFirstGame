#pragma once

#include "types.h"
#include "corestats.h"

#include <string>

struct Buff {
    Buff();

    Buff(const char* name, statustype str = 0, statustype intel = 0, statustype agi = 0,
        statustype arm = 0, statustype elres = 0, uint16_t dur = 2u, bool isdebuff = false);

    Buff(const char* name, CoreStats cs, uint16_t dur = 2u, bool isdebuff = false);

    std::string Name;
    uint16_t Duration;
    CoreStats BuffedStats;
    bool isDebuff;

    Buff& operator=(const char* name);
};