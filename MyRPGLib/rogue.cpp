#include "MyRPGLib/rogue.h"
#include "MyRPGLib/pc_macros.h"

Rogue::Rogue() : PlayerCharacterDelegate() {
    HP = std::make_unique<PointWell>(BASEHP, BASEHP);
    EP = std::make_unique<PointWell>(BASEEP, BASEEP);  // be sure to init before PCCONSTRUCT MACRO
    PCCONSTRUCT

}
void Rogue::level_char_up() noexcept {
    LEVELUP
        if (GetLevel() == 2) {
            Abilities.emplace_back(new Ability("Blink Attack", 6u, nullptr, 2u, 3u, ABILITYTARGET::ENEMY, ABILITYSCALER::AGI, ABILITYCOSTTYPE::EP));
        } else if (GetLevel() == 4) {
            Abilities.emplace_back(new Ability("Dagger Throw", 8u, nullptr, 3u, 3u, ABILITYTARGET::ENEMY, ABILITYSCALER::AGI, ABILITYCOSTTYPE::EP));
        } else if (GetLevel() == 6) {
            Abilities.emplace_back(new Ability("Cutthroat", 10u, nullptr, 5u, 3u, ABILITYTARGET::ENEMY, ABILITYSCALER::AGI, ABILITYCOSTTYPE::EP));
        }
}