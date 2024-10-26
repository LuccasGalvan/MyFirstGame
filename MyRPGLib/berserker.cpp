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
            Abilities.emplace_back(new Ability("Que Raivaa''", 6u, nullptr, 2u, 2u, ABILITYTARGET::ENEMY, ABILITYSCALER::STR, ABILITYCOSTTYPE::EP));
        } else if (GetLevel() == 3) {
            Abilities.emplace_back(new Ability("Death Wish", 8u, nullptr, 4u, 4u, ABILITYTARGET::ENEMY, ABILITYSCALER::STR, ABILITYCOSTTYPE::HP));
        }
        else if (GetLevel() == 5) {
            Abilities.emplace_back(new Ability("Enraged Attack", 8u, nullptr, 2u, 2u, ABILITYTARGET::ENEMY, ABILITYSCALER::STR, ABILITYCOSTTYPE::EP));
        }
}