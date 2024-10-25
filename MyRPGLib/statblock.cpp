#include "MyRPGLib/statblock.h"

StatBlock::StatBlock(statustype s, statustype i, statustype a, statustype arm, statustype elres) {
    base.Strength = s;
    base.Intellect = i;
    base.Agility = a;
    base.Armor = arm;
    base.ElementRes = elres;
}
statustype StatBlock::GetBaseStrength() { return   base.Strength; }
statustype StatBlock::GetBaseIntellect() { return  base.Intellect; }
statustype StatBlock::GetBaseAgility() { return    base.Agility; }
statustype StatBlock::GetBaseArmor() { return      base.Armor; }
statustype StatBlock::GetBaseElementRes() { return base.ElementRes; }

statustype StatBlock::GetTotalStrength() { return   base.Strength + fromBuffs.Strength; }
statustype StatBlock::GetTotalIntellect() { return  base.Intellect + fromBuffs.Intellect; }
statustype StatBlock::GetTotalAgility() { return    base.Agility + fromBuffs.Agility; }
statustype StatBlock::GetTotalArmor() { return      base.Armor + fromBuffs.Armor; }
statustype StatBlock::GetTotalElementRes() { return base.ElementRes + fromBuffs.ElementRes; }

void StatBlock::add_or_refresh_buff(Buff b) {
    for (auto& _buff : Buffs) {
        if (b.Name == _buff.Name) {
            _buff.Duration = b.Duration;
            return;
        }
    }
    Buffs.push_back(b);
    recalculate_buffs();
}
void StatBlock::increase_stats(statustype s, statustype i, statustype a, statustype arm, statustype elres) {
    base.Strength += s;
    base.Intellect += i;
    base.Agility += a;
    base.Armor += arm;
    base.ElementRes += elres;
}
void StatBlock::increase_stats(CoreStats cs) {
    base += cs;
}
void StatBlock::recalculate_buffs() {
    CoreStats tmp_total;
    for (const auto& b : Buffs) {
        if (b.isDebuff) {
            tmp_total -= b.BuffedStats;
        }
        else {
            tmp_total += b.BuffedStats;
        }
    }
    fromBuffs = tmp_total;
}