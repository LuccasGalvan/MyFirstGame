#pragma once

#include "types.h"
#include "statblock.h"
#include "pointwell.h"
#include "ability.h"
#include "item.h"
#include "potion.h"
#include "armor.h"
#include "weapon.h"
#include "buff.h"
#include "item_manager.h"
#include <memory>
#include <string>
#include <vector>
#include <algorithm>
#include "random.h"

class PlayerCharacterDelegate : public StatBlock {
public:
	static const exptype LEVEL2AT = 100u;
	PlayerCharacterDelegate();
	virtual ~PlayerCharacterDelegate() = 0;
	[[nodiscard]] leveltype GetLevel() const noexcept;
	[[nodiscard]] exptype GetCurrentEXP() const noexcept;
	[[nodiscard]] exptype GetEXPToNextLevel() const noexcept;
	void GiveEXP(const exptype amt) noexcept;
	std::unique_ptr<PointWell> HP;
	std::unique_ptr<PointWell> MP;
	std::unique_ptr<PointWell> EP;
	std::unique_ptr<PointWell> RP;
	std::vector<Ability*> Abilities;
	std::vector<Buff> Buffs;
private:
	exptype _current_exp;
	exptype _exp_to_next_level;
	virtual void level_char_up() noexcept = 0;
	leveltype _current_level;
	[[nodiscard]] const bool check_if_leveled() noexcept;
};

class PlayerCharacter final {
public:
	PlayerCharacter(PlayerCharacterDelegate* pc);
	~PlayerCharacter();

	// Getters
	[[nodiscard]] const leveltype GetLevel() const noexcept;
	[[nodiscard]] const exptype GetCurrentEXP() const noexcept;
	[[nodiscard]] const exptype GetEXPToNextLevel() const noexcept;
	[[nodiscard]] const bool IsMaxHealth() const noexcept;
	[[nodiscard]] const welltype GetCurrentHP() const noexcept;
	[[nodiscard]] const welltype GetMaxHP() const noexcept;
	[[nodiscard]] const welltype GetCurrentMP() const noexcept;
	[[nodiscard]] const welltype GetMaxMP() const noexcept;
	[[nodiscard]] const welltype GetCurrentEP() const noexcept;
	[[nodiscard]] const welltype GetMaxEP() const noexcept;
	[[nodiscard]] const welltype GetCurrentRP() const noexcept;
	[[nodiscard]] const welltype GetMaxRP() const noexcept;
	[[nodiscard]] const statustype GetBaseStrength() const noexcept;
	[[nodiscard]] const statustype GetBaseIntellect() const noexcept;
	[[nodiscard]] const statustype GetBaseAgility() const noexcept;
	[[nodiscard]] const statustype GetBaseArmor() const noexcept;
	[[nodiscard]] const statustype GetBaseElementRes() const noexcept;
	[[nodiscard]] const statustype GetTotalStrength() const noexcept;
	[[nodiscard]] const statustype GetTotalIntellect() const noexcept;
	[[nodiscard]] const statustype GetTotalAgility() const noexcept;
	[[nodiscard]] const statustype GetTotalArmor() const noexcept;
	[[nodiscard]] const statustype GetTotalElementRes() const noexcept;
	[[nodiscard]] const std::vector<Ability*> GetAbilityList() const noexcept;
	[[nodiscard]] const std::vector<Buff> GetBuffList() const noexcept;
	[[nodiscard]] const std::vector<Item*> GetBackpackList() const noexcept;
	[[nodiscard]] const Armor* GetEquippedArmorAt(unsigned long long i) const noexcept;
	[[nodiscard]] const Weapon* GetEquippedWeaponAt(unsigned long long i) const noexcept;
	[[nodiscard]] const damagetype MeleeAttack() const;
	[[nodiscard]] const damagetype RangedAttack() const;

	// Modifiers
	void GainEXP(exptype amt) noexcept;
	void TakeDamage(welltype amt) noexcept;
	void Heal(welltype amt) noexcept;
	void ApplyBuff(Buff _buff) noexcept;

	void ReduceMP(welltype amt) noexcept;
	void IncreaseMP(welltype amt) noexcept;
	void ReduceEP(welltype amt) noexcept;
	void IncreaseEP(welltype amt) noexcept;
	void ReduceRP(welltype amt) noexcept;
	void IncreaseRP(welltype amt) noexcept;

private:
	PlayerCharacterDelegate* _player_class;
	std::vector<Item*> _backpack;

	Item* _equipped_armor[(unsigned long long)ARMORSLOT::NUM_SLOTS];
	Item* _equipped_weapons[(unsigned long long)WEAPONSLOT::NUM_SLOTS];

	void move_to_backpack(Item* item_to_move) noexcept;
	void cleanup_backpack() noexcept;
	friend class ItemManager;

	// deleted constructors
	PlayerCharacter() = delete;
	PlayerCharacter(const PlayerCharacter&) = delete;
	PlayerCharacter(const PlayerCharacter&&) = delete;
};