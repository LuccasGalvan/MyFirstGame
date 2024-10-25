#include "MyRPGLib/cleric.h"
#include "MyRPGLib/pc_macros.h"

Cleric::Cleric() : PlayerCharacterDelegate() {
    HP = std::make_unique<PointWell>(BASEHP, BASEHP);
    MP = std::make_unique<PointWell>(BASEMP, BASEMP);
    PCCONSTRUCT
        Abilities.emplace_back(new Ability("Heal", 2u, nullptr, 2u, 1u, ABILITYTARGET::ALLY, ABILITYSCALER::INT, ABILITYCOSTTYPE::MP));
}

void Cleric::level_char_up() noexcept {
    LEVELUP
        if (GetLevel() == 2) {
            Abilities.emplace_back(new Ability("Smite", 2u, nullptr, 2u, 1u, ABILITYTARGET::ENEMY, ABILITYSCALER::INT, ABILITYCOSTTYPE::MP));
        }
}