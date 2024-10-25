#pragma once

#include "types.h"
#include "playercharacter.h"

class Cleric : public PlayerCharacterDelegate {
public:
    static constexpr welltype BASEHP = (welltype)14u;
    static constexpr welltype BASEMP = (welltype)10u;
    static constexpr welltype BASEEP = (welltype)0u;
    static constexpr welltype BASERP = (welltype)0u;
    static constexpr statustype BASESTR = (statustype)3u;
    static constexpr statustype BASEINT = (statustype)5u;
    static constexpr statustype BASEAGI = (statustype)1u;
    Cleric();
private:
    void level_char_up() noexcept override;
};