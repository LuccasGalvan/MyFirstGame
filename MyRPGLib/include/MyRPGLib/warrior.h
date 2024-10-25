#pragma once

#include "playercharacter.h"

class Warrior : public PlayerCharacterDelegate {
public:
    static constexpr welltype BASEHP = (welltype)18u;
    static constexpr welltype BASEMP = (welltype)0u;
    static constexpr welltype BASEEP = (welltype)0u;
    static constexpr welltype BASERP = (welltype)0u;
    static constexpr statustype BASESTR = (statustype)6u;
    static constexpr statustype BASEINT = (statustype)2u;
    static constexpr statustype BASEAGI = (statustype)2u;
    Warrior();
private:
    void level_char_up() noexcept override;
};