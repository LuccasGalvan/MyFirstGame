#pragma once

#include "memory"

#include "types.h"
#include "pointwell.h"

class Monster{
public:
	Monster(welltype hp, damagetype min, damagetype max);
	[[nodiscard]] const damagetype Attack() const;
	PointWell HP;
	[[nodiscard]] const damagetype GetMinDamage() const noexcept;
	[[nodiscard]] const damagetype GetMaxDamage() const noexcept;
private:
	damagetype MinDamage, MaxDamage;

	Monster() = delete;
	Monster(const Monster&) = delete;
	Monster(const Monster&&) = delete;
};

