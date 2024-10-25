#pragma once

#include "types.h"
//Point well started as a simple HP system for the characters, but it was turned into an _anything that involves points_
//system, it will probably futurely be used for mana/rage/energy systems as well. haha. as .... well.

class PointWell {
public:
    [[nodiscard]] const welltype GetMax() const noexcept;
    [[nodiscard]] const welltype GetCurrent() const noexcept;
    [[nodiscard]] const bool IsFull() const noexcept;

    void SetMax(welltype new_max) noexcept;
    void ReduceCurrent(welltype damage) noexcept;
    void IncreaseCurrent(welltype amount) noexcept;

    PointWell();
    PointWell(welltype c, welltype m);

private:
    welltype _current;
    welltype _max;
};