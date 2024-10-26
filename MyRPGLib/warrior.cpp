#include "MyRPGLib/warrior.h"
#include "MyRPGLib/pc_macros.h"

Warrior::Warrior() : PlayerCharacterDelegate() {
    HP = std::make_unique<PointWell>(BASEHP, BASEHP);
    PCCONSTRUCT

}
void Warrior::level_char_up() noexcept {
    LEVELUP
        if (GetLevel() == 2) {
            Abilities.emplace_back(new Ability("Power Attack", 4u, nullptr, 0u, 3u, ABILITYTARGET::ENEMY, ABILITYSCALER::STR, ABILITYCOSTTYPE::NONE));
        }
        else if (GetLevel() == 3) {
            Abilities.emplace_back(new Ability("Healing Surge", 4u, nullptr, 0u, 4u, ABILITYTARGET::SELF, ABILITYSCALER::NONE, ABILITYCOSTTYPE::NONE));
        }
        else if (GetLevel() == 5) {
            Abilities.emplace_back(new Ability("Skull Crusher", 7u, nullptr, 0u, 4u, ABILITYTARGET::ENEMY, ABILITYSCALER::STR, ABILITYCOSTTYPE::NONE));
        }
}