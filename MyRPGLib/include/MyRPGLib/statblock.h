#pragma once

#include "types.h"
#include "buff.h"
#include <vector>

class StatBlock {
    CoreStats base;
    CoreStats fromBuffs;
public:
    StatBlock(statustype s = 1, statustype i = 1, statustype a = 1, statustype arm = 0, statustype elres = 0);
    statustype GetBaseStrength();
    statustype GetBaseIntellect();
    statustype GetBaseAgility();
    statustype GetBaseArmor();
    statustype GetBaseElementRes();
    statustype GetTotalStrength();
    statustype GetTotalIntellect();
    statustype GetTotalAgility();
    statustype GetTotalArmor();
    statustype GetTotalElementRes();
protected:
    std::vector<Buff> Buffs;
    void add_or_refresh_buff(Buff b);
    void increase_stats(statustype s = 0, statustype i = 0, statustype a = 0, statustype arm = 0, statustype elres = 0);
    void increase_stats(CoreStats cs);
private:
    void recalculate_buffs();
};