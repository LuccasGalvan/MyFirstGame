#include "MyRPGLib/playerCharacter.h"

PlayerCharacterDelegate::~PlayerCharacterDelegate() {}


void PlayerCharacterDelegate::GiveEXP(const exptype amt) noexcept {
    _current_exp += amt;
    while (check_if_leveled()) {}
}

PlayerCharacterDelegate::PlayerCharacterDelegate() : StatBlock(0u, 0u) {
    _current_level = (leveltype)1u;
    _current_exp = (exptype)0u;
    _exp_to_next_level = LEVEL2AT;
}

[[nodiscard]] leveltype PlayerCharacterDelegate::GetLevel() const noexcept {
    return _current_level;
}

[[nodiscard]] exptype PlayerCharacterDelegate::GetCurrentEXP() const noexcept {
    return _current_exp;
}

[[nodiscard]] exptype PlayerCharacterDelegate::GetEXPToNextLevel() const noexcept {
    return _exp_to_next_level;
}

[[nodiscard]] const bool PlayerCharacterDelegate::check_if_leveled() noexcept {
    static const leveltype LEVELSCALAR = 2u;
    if (_current_exp >= _exp_to_next_level) {
        exptype overflowEXP = _current_exp - _exp_to_next_level;
        _current_level++;
        level_char_up();
        _exp_to_next_level *= LEVELSCALAR;
        _current_exp = 0u;
        GiveEXP(overflowEXP);
        return true;
    }
    return false;
}

void PlayerCharacter::move_to_backpack(Item* item_to_move) noexcept {
    _backpack.push_back(item_to_move);
}

void PlayerCharacter::cleanup_backpack() noexcept {
    const auto to_remove = std::stable_partition(_backpack.begin(), _backpack.end(),
        [](const Item* i) -> bool { return !i->GetMarkedForDeletion(); }
    );
    std::for_each(to_remove, _backpack.end(), [](Item*& i) { ItemManager::DeleteItem(i); });
    _backpack.erase(to_remove, _backpack.end());

    const auto to_remove_pointer = std::stable_partition(_backpack.begin(), _backpack.end(),
        [](const Item* i) -> bool { return !i->GetMarkedAsEquipped(); }
    );

    _backpack.erase(to_remove_pointer, _backpack.end());
}

PlayerCharacter::PlayerCharacter(PlayerCharacterDelegate* pc) : _player_class(pc) {
    auto i = 0;
    for (i = 0; i < (unsigned long long)ARMORSLOT::NUM_SLOTS; i++) {
        _equipped_armor[i] = nullptr;
    }
    for (i = 0; i < (unsigned long long)WEAPONSLOT::NUM_SLOTS; i++) {
        _equipped_weapons[i] = nullptr;
    }
}

PlayerCharacter::~PlayerCharacter() {
    delete _player_class;
    _player_class = nullptr;
    auto i = 0;
    for (i = 0; i < (unsigned long long)ARMORSLOT::NUM_SLOTS; i++) {
        if (_equipped_armor[i]) {
            ItemManager::DeleteItem(_equipped_armor[i]);
        }
    }
    for (i = 0; i < (unsigned long long)WEAPONSLOT::NUM_SLOTS; i++) {
        if (_equipped_weapons[i]) {
            ItemManager::DeleteItem(_equipped_weapons[i]);
        }
    }
}

// Getters
const leveltype PlayerCharacter::GetLevel() const noexcept { return _player_class->GetLevel(); }
const exptype PlayerCharacter::GetCurrentEXP() const noexcept { return _player_class->GetCurrentEXP(); }
const exptype PlayerCharacter::GetEXPToNextLevel() const noexcept { return _player_class->GetEXPToNextLevel(); }
const bool PlayerCharacter::IsMaxHealth() const noexcept { return _player_class->HP->IsFull(); }
const welltype PlayerCharacter::GetCurrentHP() const noexcept { return _player_class->HP->GetCurrent(); }
const welltype PlayerCharacter::GetMaxHP() const noexcept { return _player_class->HP->GetMax(); }
const welltype PlayerCharacter::GetCurrentMP() const noexcept {
    if (_player_class->MP)
        return _player_class->MP->GetCurrent();
    else
        return 0;
}
const welltype PlayerCharacter::GetMaxMP() const noexcept {
    if (_player_class->MP)
        return _player_class->MP->GetMax();
    else
        return 0;
}
const welltype PlayerCharacter::GetCurrentEP() const noexcept {
    if (_player_class->EP)
        return _player_class->EP->GetCurrent();
    else
        return 0;
}
const welltype PlayerCharacter::GetMaxEP() const noexcept {
    if (_player_class->EP)
        return _player_class->EP->GetMax();
    else
        return 0;
} 
const welltype PlayerCharacter::GetCurrentRP() const noexcept {
    if (_player_class->RP)
        return _player_class->RP->GetCurrent();
    else
        return 0;
}
const welltype PlayerCharacter::GetMaxRP() const noexcept {
    if (_player_class->RP)
        return _player_class->RP->GetMax();
    else
        return 0;
}

const statustype PlayerCharacter::GetBaseStrength() const noexcept { return _player_class->GetBaseStrength(); }
const statustype PlayerCharacter::GetBaseIntellect() const noexcept { return _player_class->GetBaseIntellect(); }
const statustype PlayerCharacter::GetBaseAgility() const noexcept { return _player_class->GetBaseAgility(); }
const statustype PlayerCharacter::GetBaseArmor() const noexcept { return _player_class->GetBaseArmor(); }
const statustype PlayerCharacter::GetBaseElementRes() const noexcept { return _player_class->GetBaseElementRes(); }
const statustype PlayerCharacter::GetTotalStrength() const noexcept {
    statustype str_from_armor = 0;
    {
        Armor* armor = nullptr;
        for (auto i = 0; i < (unsigned long long)ARMORSLOT::NUM_SLOTS; i++) {
            ItemManager::CastItemToArmor(_equipped_armor[i], armor);
            if (armor) {
                str_from_armor += armor->Stats.Strength;
            }
            armor = nullptr;
        }
    }
    statustype str_from_weapons = 0;
    {
        Weapon* weapon = nullptr;
        for (auto i = 0; i < (unsigned long long)WEAPONSLOT::NUM_SLOTS; i++) {
            ItemManager::CastItemToWeapon(_equipped_weapons[i], weapon);
            if (weapon) {
                str_from_weapons += weapon->Stats.Strength;
            }
            weapon = nullptr;
        }
    }
    return _player_class->GetTotalStrength() + str_from_armor + str_from_weapons;
}
const statustype PlayerCharacter::GetTotalIntellect() const noexcept {
    statustype int_from_armor = 0;
    {
        Armor* armor = nullptr;
        for (auto i = 0; i < (unsigned long long)ARMORSLOT::NUM_SLOTS; i++) {
            ItemManager::CastItemToArmor(_equipped_armor[i], armor);
            if (armor) {
                int_from_armor += armor->Stats.Intellect;
            }
            armor = nullptr;
        }
    }
    statustype int_from_weapons = 0;
    {
        Weapon* weapon = nullptr;
        for (auto i = 0; i < (unsigned long long)WEAPONSLOT::NUM_SLOTS; i++) {
            ItemManager::CastItemToWeapon(_equipped_weapons[i], weapon);
            if (weapon) {
                int_from_weapons += weapon->Stats.Intellect;
            }
            weapon = nullptr;
        }
    }
    return _player_class->GetTotalIntellect() + int_from_armor + int_from_weapons;
}
const statustype PlayerCharacter::GetTotalAgility() const noexcept {
    statustype agil_from_armor = 0;
    {
        Armor* armor = nullptr;
        for (auto i = 0; i < (unsigned long long)ARMORSLOT::NUM_SLOTS; i++) {
            ItemManager::CastItemToArmor(_equipped_armor[i], armor);
            if (armor) {
                agil_from_armor += armor->Stats.Agility;
            }
            armor = nullptr;
        }
    }
    statustype agil_from_weapons = 0;
    {
        Weapon* weapon = nullptr;
        for (auto i = 0; i < (unsigned long long)WEAPONSLOT::NUM_SLOTS; i++) {
            ItemManager::CastItemToWeapon(_equipped_weapons[i], weapon);
            if (weapon) {
                agil_from_weapons += weapon->Stats.Agility;
            }
            weapon = nullptr;
        }
    }
    return _player_class->GetTotalAgility() + agil_from_armor + agil_from_weapons;
}
const statustype PlayerCharacter::GetTotalArmor() const noexcept {
    // get all armor from equipped armor
    statustype armor_from_armor = 0;
    {
        Armor* armor = nullptr;
        for (auto i = 0; i < (unsigned long long)ARMORSLOT::NUM_SLOTS; i++) {
            ItemManager::CastItemToArmor(_equipped_armor[i], armor);
            if (armor) {
                armor_from_armor += armor->Stats.Armor;
            }
            armor = nullptr;
        }
    }
    statustype armor_from_weapons = 0;
    {
        Weapon* weapon = nullptr;
        for (auto i = 0; i < (unsigned long long)WEAPONSLOT::NUM_SLOTS; i++) {
            ItemManager::CastItemToWeapon(_equipped_weapons[i], weapon);
            if (weapon) {
                armor_from_weapons += weapon->Stats.Armor;
            }
            weapon = nullptr;
        }
    }
    return _player_class->GetTotalArmor() + armor_from_armor + armor_from_weapons;
}
const statustype PlayerCharacter::GetTotalElementRes() const noexcept {
    statustype resist_from_armor = 0;
    {
        Armor* armor = nullptr;
        for (auto i = 0; i < (unsigned long long)ARMORSLOT::NUM_SLOTS; i++) {
            ItemManager::CastItemToArmor(_equipped_armor[i], armor);
            if (armor) {
                resist_from_armor += armor->Stats.ElementRes;
            }
            armor = nullptr;
        }
    }
    statustype elres_from_weapons = 0;
    {
        Weapon* weapon = nullptr;
        for (auto i = 0; i < (unsigned long long)WEAPONSLOT::NUM_SLOTS; i++) {
            ItemManager::CastItemToWeapon(_equipped_weapons[i], weapon);
            if (weapon) {
                elres_from_weapons += weapon->Stats.ElementRes;
            }
            weapon = nullptr;
        }
    }
    return _player_class->GetTotalElementRes() + resist_from_armor + elres_from_weapons;
}
const std::vector<Ability*> PlayerCharacter::GetAbilityList() const noexcept { return _player_class->Abilities; }
const std::vector<Buff> PlayerCharacter::GetBuffList() const noexcept { return _player_class->Buffs; }
const std::vector<Item*> PlayerCharacter::GetBackpackList() const noexcept { return _backpack; }
const Armor* PlayerCharacter::GetEquippedArmorAt(unsigned long long i) const noexcept {
    if (!_equipped_armor[i]) return nullptr;
    return (dynamic_cast<const Armor*>(_equipped_armor[i]->GetData()));
}
const Weapon* PlayerCharacter::GetEquippedWeaponAt(unsigned long long i) const noexcept {
    if (!_equipped_weapons[i]) return nullptr;
    return (dynamic_cast<const Weapon*>(_equipped_weapons[i]->GetData()));
}
const damagetype PlayerCharacter::MeleeAttack() const{
    damagetype tmp_damage_done = 0;

    const Weapon* equipped_weapon = GetEquippedWeaponAt((unsigned long long)WEAPONSLOT::MELEE);

    //if weapon exists gets the damage, else the damage stays as 0.
    if (equipped_weapon) {
        tmp_damage_done = Random::NTK(equipped_weapon->MinDamage, equipped_weapon->MaxDamage);
    }
    else {
        tmp_damage_done = 1;
    }

    //add 1/4 of strength as melee damage
    tmp_damage_done += damagetype(GetTotalStrength() / 4.f);

    return tmp_damage_done;
}
const damagetype PlayerCharacter::RangedAttack() const {
    damagetype tmp_damage_done = 0;

    const Weapon* equipped_weapon = GetEquippedWeaponAt((unsigned long long)WEAPONSLOT::RANGED);

    //if weapon exists gets the damage, else the damage stays as 0.
    if (equipped_weapon) {
        tmp_damage_done = Random::NTK(equipped_weapon->MinDamage, equipped_weapon->MaxDamage);
        tmp_damage_done += damagetype(GetTotalAgility() / 4.f); //add 1/4 of agility as ranged damage
    }

    return tmp_damage_done;
}

// Modifiers
void PlayerCharacter::GainEXP(exptype amt) noexcept { _player_class->GiveEXP(amt); }
void PlayerCharacter::TakeDamage(welltype amt) noexcept { _player_class->HP->ReduceCurrent(amt); }
void PlayerCharacter::Heal(welltype amt) noexcept { _player_class->HP->IncreaseCurrent(amt); }
void PlayerCharacter::ApplyBuff(Buff _buff) noexcept {
    _player_class->Buffs.push_back(_buff);
}

void PlayerCharacter::ReduceMP(welltype amt) noexcept { _player_class->MP->ReduceCurrent(amt); }
void PlayerCharacter::IncreaseMP(welltype amt) noexcept { _player_class->MP->IncreaseCurrent(amt); }

void PlayerCharacter::ReduceEP(welltype amt) noexcept { _player_class->EP->ReduceCurrent(amt); }
void PlayerCharacter::IncreaseEP(welltype amt) noexcept { _player_class->EP->IncreaseCurrent(amt); }

void PlayerCharacter::ReduceRP(welltype amt) noexcept { _player_class->RP->ReduceCurrent(amt); }
void PlayerCharacter::IncreaseRP(welltype amt) noexcept { _player_class->RP->IncreaseCurrent(amt); }