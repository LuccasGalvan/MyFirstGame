#pragma once

#include "types.h"

struct CoreStats {
    statustype Strength;
    statustype Intellect;
    statustype Agility;
    statustype Armor;
    statustype ElementRes;

    CoreStats();

    CoreStats(statustype s, statustype i, statustype ag, statustype arm, statustype eres);

    CoreStats(statustype all);

    CoreStats& operator+=(const CoreStats& rhs);

    CoreStats& operator-=(const CoreStats& rhs);

    CoreStats& operator=(const statustype& rhs);
};