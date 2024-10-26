#include "MyRPGLib/wizard.h"
#include "MyRPGLib/pc_macros.h"

Wizard::Wizard() : PlayerCharacterDelegate() {
    HP = std::make_unique<PointWell>(BASEHP, BASEHP);
    MP = std::make_unique<PointWell>(BASEMP, BASEMP);
    PCCONSTRUCT
        Abilities.emplace_back(new Ability("Firebolt", 4u, nullptr, 4u, 3u, ABILITYTARGET::ENEMY, ABILITYSCALER::INT, ABILITYCOSTTYPE::MP));
}
void Wizard::level_char_up() noexcept {
    LEVELUP
        if (GetLevel() == 2) {
            Abilities.emplace_back(new Ability("IceBolt", 6u, nullptr, 5u, 3u, ABILITYTARGET::ENEMY, ABILITYSCALER::INT, ABILITYCOSTTYPE::MP));
            increase_stats(0, 1);
        } 
        else if (GetLevel() == 4) {
            Abilities.emplace_back(new Ability("Fireball", 8u, nullptr, 8u, 6u, ABILITYTARGET::ENEMY, ABILITYSCALER::INT, ABILITYCOSTTYPE::MP));
            increase_stats(0, 1);
        }
        else if (GetLevel() == 6) {
            Abilities.emplace_back(new Ability("Meteor strike", 6u, nullptr, 10u, 8u, ABILITYTARGET::ENEMY, ABILITYSCALER::INT, ABILITYCOSTTYPE::MP));
            increase_stats(0, 1);
        }

}