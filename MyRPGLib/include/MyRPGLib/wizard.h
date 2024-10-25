#pragma once

#include "types.h"
#include "playercharacter.h"

class Wizard : public PlayerCharacterDelegate {
public:
    static constexpr welltype BASEHP = (welltype)10u;
    static constexpr welltype BASEMP = (welltype)14u;
    static constexpr welltype BASEEP = (welltype)0u;
    static constexpr welltype BASERP = (welltype)0u;
    static constexpr statustype BASESTR = (statustype)1u;
    static constexpr statustype BASEINT = (statustype)8u;
    static constexpr statustype BASEAGI = (statustype)2u;
    Wizard();
private:
    void level_char_up() noexcept override;
};