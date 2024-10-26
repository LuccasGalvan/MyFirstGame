#include "MyRPGLib/cleric.h"
#include "MyRPGLib/pc_macros.h"

Cleric::Cleric() : PlayerCharacterDelegate() {
    HP = std::make_unique<PointWell>(BASEHP, BASEHP);
    MP = std::make_unique<PointWell>(BASEMP, BASEMP);
    PCCONSTRUCT
        Abilities.emplace_back(new Ability("Heal", 2u, nullptr, 2u, 2u, ABILITYTARGET::ALLY, ABILITYSCALER::INT, ABILITYCOSTTYPE::MP));
}

void Cleric::level_char_up() noexcept {
    LEVELUP
        if (GetLevel() == 2) {
            Abilities.emplace_back(new Ability("Smite", 2u, nullptr, 2u, 2u, ABILITYTARGET::ENEMY, ABILITYSCALER::INT, ABILITYCOSTTYPE::MP));
        } else if (GetLevel() == 4) {
            Abilities.emplace_back(new Ability("Mace Strike", 5u, nullptr, 2u, 4u, ABILITYTARGET::ENEMY, ABILITYSCALER::INT, ABILITYCOSTTYPE::NONE));
        }
}