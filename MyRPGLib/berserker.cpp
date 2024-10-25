#include "MyRPGLib/berserker.h"
#include "MyRPGLib/pc_macros.h"

Berserker::Berserker() : PlayerCharacterDelegate() {
    HP = std::make_unique<PointWell>(BASEHP, BASEHP);
    EP = std::make_unique<PointWell>(BASEEP, BASEEP);  // be sure to init before PCCONSTRUCT MACRO
    RP = std::make_unique<PointWell>(BASERP, BASERP);  // be sure to init before PCCONSTRUCT MACRO
    PCCONSTRUCT
}

void Berserker::level_char_up() noexcept {
    LEVELUP
        if (GetLevel() == 2) {
            Abilities.emplace_back(new Ability("RageAttack", 6u, nullptr, 1u, 2u, ABILITYTARGET::ENEMY, ABILITYSCALER::STR, ABILITYCOSTTYPE::EP));
        }
}