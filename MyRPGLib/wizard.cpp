#include "MyRPGLib/wizard.h"
#include "MyRPGLib/pc_macros.h"

Wizard::Wizard() : PlayerCharacterDelegate() {
    HP = std::make_unique<PointWell>(BASEHP, BASEHP);
    MP = std::make_unique<PointWell>(BASEMP, BASEMP);
    PCCONSTRUCT
        Abilities.emplace_back(new Ability("Firebolt", 4u, nullptr, 3u, 1u, ABILITYTARGET::ENEMY, ABILITYSCALER::INT, ABILITYCOSTTYPE::MP));
}
void Wizard::level_char_up() noexcept {
    LEVELUP
        if (GetLevel() == 2) {
            Abilities.emplace_back(new Ability("IceBolt", 6u, nullptr, 3u, 1u, ABILITYTARGET::ENEMY, ABILITYSCALER::INT, ABILITYCOSTTYPE::MP));
            increase_stats(0, 1);
        }
}