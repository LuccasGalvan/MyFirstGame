#pragma once

#include "types.h"
#include "playercharacter.h"

class Berserker : public PlayerCharacterDelegate {
public:
    static constexpr welltype BASEHP = (welltype)20u;
    static constexpr welltype BASEMP = (welltype)0u;
    static constexpr welltype BASEEP = (welltype)10u;
    static constexpr welltype BASERP = (welltype)0u;
    static constexpr statustype BASESTR = (statustype)3u;
    static constexpr statustype BASEINT = (statustype)3u;
    static constexpr statustype BASEAGI = (statustype)6u;
    Berserker();
private:
    void level_char_up() noexcept override;
};