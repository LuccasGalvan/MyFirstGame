#include "pch.h"

Player* MainCharacter = nullptr;
Enemy* CurrentMonster = nullptr;
int monstersDefeated = 0;

bool abilitySelection() {
	int selectedAbility = 0;
	system("CLS");

	bool actionUsed = false;
	bool done = false;
	while (!done) {
		auto currentAbilities = MainCharacter->p1.GetAbilityList();
		system("CLS");
		std::cout << "ABILITIES: \n\n";

		int abilitiesInList = 0;
		for (const auto& ability : currentAbilities) {
			if (selectedAbility == abilitiesInList)
				std::cout << "> ";
			else
				std::cout << "  ";
			std::cout << ability->GetName() << " - Cost: " << ability->GetCost();
			if(ability->RemainingCooldown != 0)
				std::cout << " Remaining Cooldown: " << ability->RemainingCooldown << std::endl;

			abilitiesInList++;
		}
		std::cout << "\n---------------------------";

		std::cin.ignore(100, '\n');
		std::cout << "\nPress b to leave ability list\n";
		std::cout << "\nw = up, s = down, u = use\n";
		char c = getchar();
		switch (c) {
		case 'b':
			done = true;
			break;
		case 'w':
			selectedAbility--;
			if (selectedAbility < 0)
				selectedAbility = 0;
			break;
		case 's':
			selectedAbility++;
			if (selectedAbility > abilitiesInList - 1)
				selectedAbility = abilitiesInList - 1;
			break;
		case 'u':
			if (currentAbilities[selectedAbility]->GetTarget() == ABILITYTARGET::ENEMY) {
				int totalDamage = 0;
				int abilityCost;
				totalDamage += currentAbilities[selectedAbility]->GetHPEffect();

				switch (currentAbilities[selectedAbility]->GetCostType()) {
				case ABILITYCOSTTYPE::MP:
					abilityCost = currentAbilities[selectedAbility]->GetCost();
					if (abilityCost > MainCharacter->p1.GetCurrentMP() || currentAbilities[selectedAbility]->RemainingCooldown > 0)
						return false;
					MainCharacter->p1.ReduceMP(abilityCost);
					currentAbilities[selectedAbility]->RemainingCooldown = currentAbilities[selectedAbility]->GetCooldown();
					break;
				case ABILITYCOSTTYPE::EP:
					abilityCost = currentAbilities[selectedAbility]->GetCost();
					if (abilityCost > MainCharacter->p1.GetCurrentEP() || currentAbilities[selectedAbility]->RemainingCooldown > 0)
						return false;
					MainCharacter->p1.ReduceEP(abilityCost);
					currentAbilities[selectedAbility]->RemainingCooldown = currentAbilities[selectedAbility]->GetCooldown();
					break;
				case ABILITYCOSTTYPE::HP:
					abilityCost = currentAbilities[selectedAbility]->GetCost();
					if (abilityCost > MainCharacter->p1.GetCurrentHP() || currentAbilities[selectedAbility]->RemainingCooldown > 0)
						return false;
					MainCharacter->p1.TakeDamage(abilityCost);
					currentAbilities[selectedAbility]->RemainingCooldown = currentAbilities[selectedAbility]->GetCooldown();
					break;
				case ABILITYCOSTTYPE::RP:
					abilityCost = currentAbilities[selectedAbility]->GetCost();
					if (abilityCost > MainCharacter->p1.GetCurrentRP() || currentAbilities[selectedAbility]->RemainingCooldown > 0)
						return false;
					MainCharacter->p1.ReduceRP(abilityCost);
					currentAbilities[selectedAbility]->RemainingCooldown = currentAbilities[selectedAbility]->GetCooldown();
					break;
				}

				switch (currentAbilities[selectedAbility]->GetScaler()) {
				case ABILITYSCALER::STR:
					totalDamage += (int)(MainCharacter->p1.GetTotalStrength()/2.f);
					break;
				case ABILITYSCALER::INT:
					totalDamage += (int)(MainCharacter->p1.GetTotalIntellect()/2.f);
					break;
				case ABILITYSCALER::AGI:
					totalDamage += (int)(MainCharacter->p1.GetTotalAgility()/2.f);
					break;
				default:
					break;
				}
				CurrentMonster->monster.HP.ReduceCurrent(totalDamage);
			}
			else {
				int totalHeal = 0;
				totalHeal += currentAbilities[selectedAbility]->GetHPEffect();

				switch (currentAbilities[selectedAbility]->GetScaler()) {
				case ABILITYSCALER::STR:
					totalHeal += (int)(MainCharacter->p1.GetTotalStrength() / 2.f);
					break;
				case ABILITYSCALER::INT:
					totalHeal += (int)(MainCharacter->p1.GetTotalIntellect() / 2.f);
					break;
				case ABILITYSCALER::AGI:
					totalHeal += (int)(MainCharacter->p1.GetTotalAgility() / 2.f);
					break;
				default:
					break;
				}

				MainCharacter->p1.Heal(totalHeal);
			}
			actionUsed = true;
			break;
		default:
			break;
		}
		if (actionUsed) {
			break;
		}
	}
	return actionUsed;
}

bool combatInventory() {
	system("CLS");

	bool actionUsed = false;
	bool done = false;
	int selectedItemNum = 0;
	while (!done) {
		system("CLS");
		auto listOfItems = MainCharacter->p1.GetBackpackList();
		std::cout << "CURRENT INVENTORY: \n"
			<< "---------------------------\n\n";

		int itemsInBackpackCount = 0;
		for (const auto& item : listOfItems) {
			if (selectedItemNum == itemsInBackpackCount)
				std::cout << "> ";
			else
				std::cout << "  ";
			std::cout << item->GetData()->Name << std::endl;
			if (ItemManager::IsItemPotion(item)) {
				Potion* potion = nullptr;
				ItemManager::CastItemToPotion(item, potion);
				if (potion)
					std::cout << "   Quantity: " << potion->Quantity << "\n";
			}
			itemsInBackpackCount++;
		}
		std::cout << "---------------------------";

		std::cin.ignore(100, '\n');
		std::cout << "\nPress i to leave inventory\n";
		std::cout << "\nw = up, s = down, u = use/equip the hovered item\n";
		char c = getchar();
		switch (c) {
		case 'i':
			done = true;
			break;
		case 'w':
			selectedItemNum--;
			if (selectedItemNum < 0)
				selectedItemNum = 0;
			break;
		case 's':
			selectedItemNum++;
			if (selectedItemNum > itemsInBackpackCount - 1)
				selectedItemNum = itemsInBackpackCount - 1;
			break;
		case 'u':
			if (listOfItems.empty())
				continue;
			if (ItemManager::IsItemPotion(listOfItems[selectedItemNum])) {
				actionUsed = ItemManager::Use(listOfItems[selectedItemNum], &(MainCharacter->p1));
			}
			else {
				actionUsed = ItemManager::Equip(listOfItems[selectedItemNum], &(MainCharacter->p1));
			}
			break;
		default:
			break;
		}

		if (actionUsed)
			break;
	}
	return actionUsed;
}

void displayCharacterSheet() {
	system("CLS");
	bool done = false;
	while(!done){
		std::cout
			<< MainCharacter->name << "'s Information\n"
			<< "------------------------------\n\n"
			<< "Level: " << MainCharacter->p1.GetLevel() << std::endl
			<< "XP: " << MainCharacter->p1.GetCurrentEXP() << "/" << MainCharacter->p1.GetEXPToNextLevel() << std::endl
			<< "HP: " << MainCharacter->p1.GetCurrentHP() << "/" << MainCharacter->p1.GetMaxHP() << std::endl
			<< "Armor: " << MainCharacter->p1.GetTotalArmor() << "  Elemental Resistance: " << MainCharacter->p1.GetTotalElementRes() << std::endl
			<< "Strength: " << MainCharacter->p1.GetTotalStrength() << std::endl
			<< "Intellect: " << MainCharacter->p1.GetTotalIntellect() << std::endl
			<< "Agility: " << MainCharacter->p1.GetTotalAgility() << std::endl
			<< "------------------------------\n\n"
			<< "Equipped Gear:\n";
		
			if (MainCharacter->p1.GetEquippedWeaponAt((unsigned long long)WEAPONSLOT::MELEE)) {
				std::string wepon_name = MainCharacter->p1.GetEquippedWeaponAt((unsigned long long)WEAPONSLOT::MELEE)->Name;
				std::cout << "\n\nMELEE: " << wepon_name 
					<< "(" 
					<< MainCharacter->p1.GetEquippedWeaponAt((unsigned long long)WEAPONSLOT::MELEE)->MinDamage 
					<< " - " 
					<< MainCharacter->p1.GetEquippedWeaponAt((unsigned long long)WEAPONSLOT::MELEE)->MaxDamage 
					<< ")";
			}
			else {
				std::cout << "\n\nMELEE : No weapon equipped.";
			}

			if (MainCharacter->p1.GetEquippedWeaponAt((unsigned long long)WEAPONSLOT::RANGED)) {
				std::string wepon_name = MainCharacter->p1.GetEquippedWeaponAt((unsigned long long)WEAPONSLOT::RANGED)->Name;
				std::cout << "\n\nRANGED: " << wepon_name
				<< " Attack Damage: ("
				<< MainCharacter->p1.GetEquippedWeaponAt((unsigned long long)WEAPONSLOT::RANGED)->MinDamage
				<< " - "
				<< MainCharacter->p1.GetEquippedWeaponAt((unsigned long long)WEAPONSLOT::RANGED)->MaxDamage
				<< ")";
			}
			else {
				std::cout << "\n\nRANGED : No weapon equipped.";
			}

			if (MainCharacter->p1.GetEquippedArmorAt((unsigned long long)ARMORSLOT::HEAD)) {
				std::string armor_name = MainCharacter->p1.GetEquippedArmorAt((unsigned long long)ARMORSLOT::HEAD)->Name;
				std::cout << "\n\nHEAD: " << armor_name
					<< "\nStats: "
					<< "\nArmor: "
					<< MainCharacter->p1.GetEquippedArmorAt((unsigned long long)ARMORSLOT::HEAD)->Stats.Armor
					<< "\nElement Resistance: "
					<< MainCharacter->p1.GetEquippedArmorAt((unsigned long long)ARMORSLOT::HEAD)->Stats.ElementRes
					<< "\nStrength: "
					<< MainCharacter->p1.GetEquippedArmorAt((unsigned long long)ARMORSLOT::HEAD)->Stats.Strength
					<< "\nIntellect: "
					<< MainCharacter->p1.GetEquippedArmorAt((unsigned long long)ARMORSLOT::HEAD)->Stats.Intellect
					<< "\nAgility: "
					<< MainCharacter->p1.GetEquippedArmorAt((unsigned long long)ARMORSLOT::HEAD)->Stats.Agility;
			}
			else {
				std::cout << "\n\nHEAD : No armor equipped.";
			}

			if (MainCharacter->p1.GetEquippedArmorAt((unsigned long long)ARMORSLOT::NECK)) {
				std::string armor_name = MainCharacter->p1.GetEquippedArmorAt((unsigned long long)ARMORSLOT::NECK)->Name;
				std::cout << "\n\nNECK: " << armor_name
					<< "\nStats: "
					<< "\nArmor: "
					<< MainCharacter->p1.GetEquippedArmorAt((unsigned long long)ARMORSLOT::NECK)->Stats.Armor
					<< "\nElement Resistance: "
					<< MainCharacter->p1.GetEquippedArmorAt((unsigned long long)ARMORSLOT::NECK)->Stats.ElementRes
					<< "\nStrength: "
					<< MainCharacter->p1.GetEquippedArmorAt((unsigned long long)ARMORSLOT::NECK)->Stats.Strength
					<< "\nIntellect: "
					<< MainCharacter->p1.GetEquippedArmorAt((unsigned long long)ARMORSLOT::NECK)->Stats.Intellect
					<< "\nAgility: "
					<< MainCharacter->p1.GetEquippedArmorAt((unsigned long long)ARMORSLOT::NECK)->Stats.Agility;
			}
			else {
				std::cout << "\n\nNECK : No armor equipped.";
			}

			if (MainCharacter->p1.GetEquippedArmorAt((unsigned long long)ARMORSLOT::CHEST)) {
				std::string armor_name = MainCharacter->p1.GetEquippedArmorAt((unsigned long long)ARMORSLOT::CHEST)->Name;
				std::cout << "\n\nCHEST: " << armor_name
					<< "\nStats: "
					<< "\nArmor: "
					<< MainCharacter->p1.GetEquippedArmorAt((unsigned long long)ARMORSLOT::CHEST)->Stats.Armor
					<< "\nElement Resistance: "
					<< MainCharacter->p1.GetEquippedArmorAt((unsigned long long)ARMORSLOT::CHEST)->Stats.ElementRes
					<< "\nStrength: "
					<< MainCharacter->p1.GetEquippedArmorAt((unsigned long long)ARMORSLOT::CHEST)->Stats.Strength
					<< "\nIntellect: "
					<< MainCharacter->p1.GetEquippedArmorAt((unsigned long long)ARMORSLOT::CHEST)->Stats.Intellect
					<< "\nAgility: "
					<< MainCharacter->p1.GetEquippedArmorAt((unsigned long long)ARMORSLOT::CHEST)->Stats.Agility;
			}
			else {
				std::cout << "\n\nCHEST : No armor equipped.";
			}

			if (MainCharacter->p1.GetEquippedArmorAt((unsigned long long)ARMORSLOT::HANDS)) {
				std::string armor_name = MainCharacter->p1.GetEquippedArmorAt((unsigned long long)ARMORSLOT::HANDS)->Name;
				std::cout << "\n\nHANDS: " << armor_name
					<< "\nStats: "
					<< "\nArmor: "
					<< MainCharacter->p1.GetEquippedArmorAt((unsigned long long)ARMORSLOT::HANDS)->Stats.Armor
					<< "\nElement Resistance: "
					<< MainCharacter->p1.GetEquippedArmorAt((unsigned long long)ARMORSLOT::HANDS)->Stats.ElementRes
					<< "\nStrength: "
					<< MainCharacter->p1.GetEquippedArmorAt((unsigned long long)ARMORSLOT::HANDS)->Stats.Strength
					<< "\nIntellect: "
					<< MainCharacter->p1.GetEquippedArmorAt((unsigned long long)ARMORSLOT::HANDS)->Stats.Intellect
					<< "\nAgility: "
					<< MainCharacter->p1.GetEquippedArmorAt((unsigned long long)ARMORSLOT::HANDS)->Stats.Agility;
			}
			else {
				std::cout << "\n\nHANDS : No armor equipped.";
			}

			if (MainCharacter->p1.GetEquippedArmorAt((unsigned long long)ARMORSLOT::RING1)) {
				std::string armor_name = MainCharacter->p1.GetEquippedArmorAt((unsigned long long)ARMORSLOT::RING1)->Name;
				std::cout << "\n\nRING 1: " << armor_name
					<< "\nStats: "
					<< "\nArmor: "
					<< MainCharacter->p1.GetEquippedArmorAt((unsigned long long)ARMORSLOT::RING1)->Stats.Armor
					<< "\nElement Resistance: "
					<< MainCharacter->p1.GetEquippedArmorAt((unsigned long long)ARMORSLOT::RING1)->Stats.ElementRes
					<< "\nStrength: "
					<< MainCharacter->p1.GetEquippedArmorAt((unsigned long long)ARMORSLOT::RING1)->Stats.Strength
					<< "\nIntellect: "
					<< MainCharacter->p1.GetEquippedArmorAt((unsigned long long)ARMORSLOT::RING1)->Stats.Intellect
					<< "\nAgility: "
					<< MainCharacter->p1.GetEquippedArmorAt((unsigned long long)ARMORSLOT::RING1)->Stats.Agility;
			}
			else {
				std::cout << "\n\nRING 1 : No armor equipped.";
			}

			if (MainCharacter->p1.GetEquippedArmorAt((unsigned long long)ARMORSLOT::RING2)) {
				std::string armor_name = MainCharacter->p1.GetEquippedArmorAt((unsigned long long)ARMORSLOT::RING2)->Name;
				std::cout << "\n\nRING 2: " << armor_name
					<< "\nStats: "
					<< "\nArmor: "
					<< MainCharacter->p1.GetEquippedArmorAt((unsigned long long)ARMORSLOT::RING2)->Stats.Armor
					<< "\nElement Resistance: "
					<< MainCharacter->p1.GetEquippedArmorAt((unsigned long long)ARMORSLOT::RING2)->Stats.ElementRes
					<< "\nStrength: "
					<< MainCharacter->p1.GetEquippedArmorAt((unsigned long long)ARMORSLOT::RING2)->Stats.Strength
					<< "\nIntellect: "
					<< MainCharacter->p1.GetEquippedArmorAt((unsigned long long)ARMORSLOT::RING2)->Stats.Intellect
					<< "\nAgility: "
					<< MainCharacter->p1.GetEquippedArmorAt((unsigned long long)ARMORSLOT::RING2)->Stats.Agility;
			}
			else {
				std::cout << "\n\nRING 2: No armor equipped.";
			}

			if (MainCharacter->p1.GetEquippedArmorAt((unsigned long long)ARMORSLOT::LEGS)) {
				std::string armor_name = MainCharacter->p1.GetEquippedArmorAt((unsigned long long)ARMORSLOT::LEGS)->Name;
				std::cout << "\n\nLEGS: " << armor_name
					<< "\nStats: "
					<< "\nArmor: "
					<< MainCharacter->p1.GetEquippedArmorAt((unsigned long long)ARMORSLOT::LEGS)->Stats.Armor
					<< "\nElement Resistance: "
					<< MainCharacter->p1.GetEquippedArmorAt((unsigned long long)ARMORSLOT::LEGS)->Stats.ElementRes
					<< "\nStrength: "
					<< MainCharacter->p1.GetEquippedArmorAt((unsigned long long)ARMORSLOT::LEGS)->Stats.Strength
					<< "\nIntellect: "
					<< MainCharacter->p1.GetEquippedArmorAt((unsigned long long)ARMORSLOT::LEGS)->Stats.Intellect
					<< "\nAgility: "
					<< MainCharacter->p1.GetEquippedArmorAt((unsigned long long)ARMORSLOT::LEGS)->Stats.Agility;
			}
			else {
				std::cout << "\n\nLEGS : No armor equipped.";
			}

			if (MainCharacter->p1.GetEquippedArmorAt((unsigned long long)ARMORSLOT::FEET)) {
				std::string armor_name = MainCharacter->p1.GetEquippedArmorAt((unsigned long long)ARMORSLOT::FEET)->Name;
				std::cout << "\n\nFEET: " << armor_name
					<< "\nStats: "
					<< "\nArmor: "
					<< MainCharacter->p1.GetEquippedArmorAt((unsigned long long)ARMORSLOT::FEET)->Stats.Armor
					<< "\nElement Resistance: "
					<< MainCharacter->p1.GetEquippedArmorAt((unsigned long long)ARMORSLOT::FEET)->Stats.ElementRes
					<< "\nStrength: "
					<< MainCharacter->p1.GetEquippedArmorAt((unsigned long long)ARMORSLOT::FEET)->Stats.Strength
					<< "\nIntellect: "
					<< MainCharacter->p1.GetEquippedArmorAt((unsigned long long)ARMORSLOT::FEET)->Stats.Intellect
					<< "\nAgility: "
					<< MainCharacter->p1.GetEquippedArmorAt((unsigned long long)ARMORSLOT::FEET)->Stats.Agility;
			}
			else {
				std::cout << "\n\nFEET : No armor equipped.";
			}
			std::cin.ignore(100, '\n');
			std::cout << "\nPress c to leave character sheet\n";
			char c = getchar();

			if (c == 'c') {
				done = true;
			}
		}
}

void openInventory() {
	system("CLS");

	bool done = false;
	int selectedItemNum = 0;
	while (!done) {
		system("CLS");
		auto listOfItems = MainCharacter->p1.GetBackpackList();
		std::cout << "CURRENT INVENTORY: \n"
			<< "---------------------------\n\n";

		int itemsInBackpackCount = 0;
		for (const auto& item : listOfItems) {
			if (selectedItemNum == itemsInBackpackCount)
				std::cout << "> ";
			else
				std::cout << "  ";
			std::cout << item->GetData()->Name << std::endl;
			if (ItemManager::IsItemPotion(item)) {
				Potion* potion = nullptr;
				ItemManager::CastItemToPotion(item, potion);
				if (potion)
					std::cout << "   Quantity: " << potion->Quantity << "\n";
			}
			itemsInBackpackCount++;
		}
		std::cout << "---------------------------";

		std::cin.ignore(100, '\n');
		std::cout << "\nPress i to leave inventory\n";
		std::cout << "\nw = up, s = down, u = use/equip the hovered item\n";
		char c = getchar();
		switch (c) {
		case 'i':
			done = true;
			break;
		case 'w': 
			selectedItemNum--;
			if (selectedItemNum < 0)
				selectedItemNum = 0;
			break;
		case 's':
			selectedItemNum++;
			if (selectedItemNum > itemsInBackpackCount - 1)
				selectedItemNum = itemsInBackpackCount - 1;
			break;
		case 'u':
			if (listOfItems.empty())
				continue;
			if (ItemManager::IsItemPotion(listOfItems[selectedItemNum]))
				ItemManager::Use(listOfItems[selectedItemNum], &(MainCharacter->p1));
			else
				ItemManager::Equip(listOfItems[selectedItemNum], &(MainCharacter->p1));
			break;
		default:
			break;
		}
	}
}

Item* dropRandomItem() {
	//8 armor items, 2 weapon types and 1 potion type -> 11 different drop types
	int drop_seed = Random::NTK(1, 100);

	if (drop_seed < 6) {
		std::string name;
		CoreStats localStats;
		int isMagical =	Random::NTK(1, 100);
		if (isMagical < 5) {
			name = "Magical Chestplate +3";
			localStats = CoreStats(3, 1, 2, 2, 2);
		}
		else if (isMagical < 20) {
			name = "Magical Chestplate +2";
			localStats = CoreStats(2, 0, 1, 2, 1);
		}
		else if (isMagical < 40) {
			name = "Magical Chestplate +1";
			localStats = CoreStats(1, 0, 0, 2, 1);
		}
		else if (isMagical < 60) {
			name = "Magical Chestplate";
			localStats = CoreStats(1, 0, 0, 1, 1);
		}
		else if (isMagical < 101) {
			name = "Chestplate";
			localStats = CoreStats(0, 0, 0, 1, 1);
		}

		return ItemManager::CreateArmor(name, localStats, ARMORSLOT::CHEST);
	}
	else if(drop_seed < 12) {
		std::string name;
		CoreStats localStats;
		int isMagical = Random::NTK(1, 100);
		if (isMagical < 5) {
			name = "Magical Leg Guards +3";
			localStats = CoreStats(3, 1, 2, 2, 2);
		}
		else if (isMagical < 20) {
			name = "Magical Leg Guards +2";
			localStats = CoreStats(2, 0, 1, 2, 1);
		}
		else if (isMagical < 40) {
			name = "Magical Leg Guards +1";
			localStats = CoreStats(1, 0, 0, 2, 1);
		}
		else if (isMagical < 60) {
			name = "Magical Leg Guards";
			localStats = CoreStats(1, 0, 0, 1, 1);
		}
		else if (isMagical < 101) {
			name = "Leg Guards";
			localStats = CoreStats(0, 0, 0, 1, 1);
		}

		return ItemManager::CreateArmor(name, localStats, ARMORSLOT::LEGS);
	}
	else if (drop_seed < 18) {
		std::string name;
		CoreStats localStats;
		int isMagical = Random::NTK(1, 100);
		if (isMagical < 5) {
			name = "Magical Helmet +3";
			localStats = CoreStats(3, 1, 2, 2, 2);
		}
		else if (isMagical < 20) {
			name = "Magical Helmet +2";
			localStats = CoreStats(2, 0, 1, 2, 1);
		}
		else if (isMagical < 40) {
			name = "Magical Helmet +1";
			localStats = CoreStats(1, 0, 0, 2, 1);
		}
		else if (isMagical < 60) {
			name = "Magical Helmet";
			localStats = CoreStats(1, 0, 0, 1, 1);
		}
		else if (isMagical < 101) {
			name = "Helmet";
			localStats = CoreStats(0, 0, 0, 1, 1);
		}

		return ItemManager::CreateArmor(name, localStats, ARMORSLOT::HEAD);
	}
	else if (drop_seed < 24) {
		std::string name;
		CoreStats localStats;
		int isMagical = Random::NTK(1, 100);
		if (isMagical < 5) {
			name = "Magical Boots +3";
			localStats = CoreStats(3, 1, 2, 2, 2);
		}
		else if (isMagical < 20) {
			name = "Magical Boots +2";
			localStats = CoreStats(2, 0, 1, 2, 1);
		}
		else if (isMagical < 40) {
			name = "Magical Boots +1";
			localStats = CoreStats(1, 0, 0, 2, 1);
		}
		else if (isMagical < 60) {
			name = "Magical Boots";
			localStats = CoreStats(1, 0, 0, 1, 1);
		}
		else if (isMagical < 101) {
			name = "Boots";
			localStats = CoreStats(0, 0, 0, 1, 1);
		}

		return ItemManager::CreateArmor(name, localStats, ARMORSLOT::FEET);
	}
	else if (drop_seed < 30) {
		std::string name;
		CoreStats localStats;
		int isMagical = Random::NTK(1, 100);
		if (isMagical < 5) {
			name = "Magical Gloves +3";
			localStats = CoreStats(3, 1, 2, 2, 2);
		}
		else if (isMagical < 20) {
			name = "Magical Gloves +2";
			localStats = CoreStats(2, 0, 1, 2, 1);
		}
		else if (isMagical < 40) {
			name = "Magical Gloves +1";
			localStats = CoreStats(1, 0, 0, 2, 1);
		}
		else if (isMagical < 60) {
			name = "Magical Gloves";
			localStats = CoreStats(1, 0, 0, 1, 1);
		}
		else if (isMagical < 101) {
			name = "Gloves";
			localStats = CoreStats(0, 0, 0, 1, 1);
		}

		return ItemManager::CreateArmor(name, localStats, ARMORSLOT::HANDS);
	}
	else if (drop_seed < 36) {
		std::string name;
		CoreStats localStats;
		int isMagical = Random::NTK(1, 100);
		if (isMagical < 5) {
			name = "Magical Agility ring +3";
			localStats = CoreStats(1, 0, 5, 2, 1);
		}
		else if (isMagical < 20) {
			name = "Magical Agility ring +2";
			localStats = CoreStats(1, 0, 4, 2, 1);
		}
		else if (isMagical < 40) {
			name = "Magical Agility ring +1";
			localStats = CoreStats(1, 0, 3, 1, 0);
		}
		else if (isMagical < 60) {
			name = "Magical Agility ring";
			localStats = CoreStats(0, 0, 2, 1, 0);
		}
		else if (isMagical < 101) {
			name = "Agility ring";
			localStats = CoreStats(0, 0, 1, 0, 0);
		}

		return ItemManager::CreateArmor(name, localStats, ARMORSLOT::RING1);
	}
	else if (drop_seed < 42) {
		std::string name;
		CoreStats localStats;
		int isMagical = Random::NTK(1, 100);
		if (isMagical < 5) {
			name = "Magical Strength ring +3";
			localStats = CoreStats(5, 0, 1, 2, 1);
		}
		else if (isMagical < 20) {
			name = "Magical Strength ring +2";
			localStats = CoreStats(4, 0, 1, 2, 1);
		}
		else if (isMagical < 40) {
			name = "Magical Strength ring +1";
			localStats = CoreStats(3, 0, 0, 2, 1);
		}
		else if (isMagical < 60) {
			name = "Magical Strength ring";
			localStats = CoreStats(2, 0, 0, 1, 1);
		}
		else if (isMagical < 101) {
			name = "Strength ring";
			localStats = CoreStats(1, 0, 0, 1, 1);
		}

		return ItemManager::CreateArmor(name, localStats, ARMORSLOT::RING2);
	}
	else if (drop_seed < 48) {
		std::string name;
		CoreStats localStats;
		int isMagical = Random::NTK(1, 100);
		if (isMagical < 5) {
			name = "Magical Amulet +3";
			localStats = CoreStats(0, 5, 0, 2, 3);
		}
		else if (isMagical < 20) {
			name = "Magical Amulet +2";
			localStats = CoreStats(0, 4, 0, 2, 2);
		}
		else if (isMagical < 40) {
			name = "Magical Amulet +1";
			localStats = CoreStats(0, 3, 0, 1, 2);
		}
		else if (isMagical < 60) {
			name = "Magical Amulet";
			localStats = CoreStats(0, 2, 0, 1, 1);
		}
		else if (isMagical < 101) {
			name = "Amulet";
			localStats = CoreStats(0, 1, 0, 1, 1);
		}

		return ItemManager::CreateArmor(name, localStats, ARMORSLOT::NECK);
	}
	else if (drop_seed < 54) {
		std::string name;
		CoreStats localStats;
		int swordSeed = Random::NTK(1, 100);
		if (swordSeed < 5) {
			name = "Magical Iron Sword +3";
			localStats = CoreStats(6, 1, 2, 0, 0);
		}
		else if (swordSeed < 20) {
			name = "Magical Iron Sword +2";
			localStats = CoreStats(5, 1, 2, 0, 0);
		}
		else if (swordSeed < 40) {
			name = "Magical Iron Sword +1";
			localStats = CoreStats(4, 0, 1, 0, 0);
		}
		else if (swordSeed < 60) {
			name = "Magical Iron Sword";
			localStats = CoreStats(3, 0, 1, 0, 0);
		}
		else if (swordSeed < 101) {
			name = "Iron Sword";
			localStats = CoreStats(2, 0, 0, 0, 0);
		}

		int minDmg = Random::NTK((3 * MainCharacter->p1.GetLevel()), (4 * MainCharacter->p1.GetLevel()));
		int maxDmg = Random::NTK((5 * MainCharacter->p1.GetLevel()), (6 * MainCharacter->p1.GetLevel()));

		return ItemManager::CreateWeapon(name, localStats, WEAPONSLOT::MELEE, minDmg, maxDmg);
	}
	else if (drop_seed < 60) {
		std::string name;
		CoreStats localStats;
		int swordSeed = Random::NTK(1, 100);
		if (swordSeed < 5) {
			name = "Magical Long Bow +3";
			localStats = CoreStats(0, 1, 6, 0, 0);
		}
		else if (swordSeed < 20) {
			name = "Magical Long Bow +2";
			localStats = CoreStats(0, 1, 5, 0, 0);
		}
		else if (swordSeed < 40) {
			name = "Magical Long Bow +1";
			localStats = CoreStats(0, 0, 4, 0, 0);
		}
		else if (swordSeed < 60) {
			name = "Magical Long Bow";
			localStats = CoreStats(0, 0, 3, 0, 0);
		}
		else if (swordSeed < 101) {
			name = "Long Bow";
			localStats = CoreStats(0, 0, 2, 0, 0);
		}

		int minDmg = Random::NTK((3 * MainCharacter->p1.GetLevel()), (4 * MainCharacter->p1.GetLevel()));
		int maxDmg = Random::NTK((5 * MainCharacter->p1.GetLevel()), (6 * MainCharacter->p1.GetLevel()));

		return ItemManager::CreateWeapon(name, localStats, WEAPONSLOT::RANGED, minDmg, maxDmg);
	}
	else if (drop_seed < 91) {
		return ItemManager::CreatePotion("Potion of Healing", Random::NTK(2, 5), Random::NTK(1, 2));
	}

	return nullptr;
}

void create_monster(Enemy*& in_out, const Player* base_calc) {
	if (!base_calc)
		return;

	if (in_out) {
		delete in_out;
		in_out = nullptr;
	}

	int lowestHP = base_calc->p1.GetLevel() * 2;
	int maxHP = base_calc->p1.GetLevel() * 8;
	
	int lowestDMG = base_calc->p1.GetLevel();
	int maxDMG = base_calc->p1.GetLevel() * 2;


	in_out = new Enemy(Random::NTK(lowestHP, maxHP), lowestDMG, maxDMG);

	in_out->xpos = Random::NTK(1, 11);
	in_out->ypos = Random::NTK(1, 11);

	while (theMap[in_out->xpos][in_out->ypos] == 'P' || theMap[in_out->xpos][in_out->ypos] == 'x') {
		in_out->xpos = Random::NTK(1, 11);
		in_out->ypos = Random::NTK(1, 11);
	}

	theMap[in_out->xpos][in_out->ypos] = 'M';
}

void enterFightSequence(Player& player1){
	if (!CurrentMonster)
		return;

	//options available per turn
	enum class FightOptions { NONE,  ATTACK, INVENTORY, ABILITY };

	while (player1.isAlive() && CurrentMonster->isAlive()) {
		FightOptions action_taken = FightOptions::NONE;

		char action = 'x';
		
		while (action_taken == FightOptions::NONE) {
			system("CLS");
			//display fight interface
			std::cout << "P         vs         M\n";
			std::cout << "hp: " << player1.p1.GetCurrentHP() << '/' << player1.p1.GetMaxHP()
				<< "      hp: " << CurrentMonster->monster.HP.GetCurrent() << '/' << CurrentMonster->monster.HP.GetMax() << std::endl;
			if (MainCharacter->p1.GetMaxMP() > -1)
				std::cout << "mp: " << player1.p1.GetCurrentMP() << '/' << player1.p1.GetMaxMP() << std::endl;
			if (MainCharacter->p1.GetMaxEP() != 0)
				std::cout << "ep: " << player1.p1.GetCurrentEP() << '/' << player1.p1.GetMaxEP() << std::endl;
			if (MainCharacter->p1.GetCurrentRP() != 0)
				std::cout << "rp: " << player1.p1.GetCurrentRP() << '/' << player1.p1.GetMaxRP() << std::endl;

			std::cout << "(a: melee attack r: ranged attack i: open inventory b: show abilities) What will you do? ";
			
			std::cin >> action;
			

			switch (action) {
			case 'a':
				CurrentMonster->monster.HP.ReduceCurrent(player1.p1.MeleeAttack());
				action_taken = FightOptions::ATTACK;
				break;
			case 'r':
				CurrentMonster->monster.HP.ReduceCurrent(player1.p1.RangedAttack());
				action_taken = FightOptions::ATTACK;
				break;
			case 'i':
				action_taken = (combatInventory()) ? FightOptions::INVENTORY : FightOptions::NONE;
				break;
			case 'b':
				action_taken = (abilitySelection()) ? FightOptions::ABILITY : FightOptions::NONE;
				break;
			default:
				break;
			}
		}

		//updating cooldowns
		auto currentAbilities = MainCharacter->p1.GetAbilityList();
		for (auto& ability : currentAbilities) {
			if (ability->RemainingCooldown > 0)
				ability->RemainingCooldown--;
		}

		if (CurrentMonster->isAlive()) {
			//monster attacks every turn
			int damage_we_take = CurrentMonster->monster.Attack();
			damage_we_take -= player1.p1.GetTotalArmor();
			if (damage_we_take < 1)
				damage_we_take = 1;

			player1.p1.TakeDamage(damage_we_take);


		}
	}

	if (player1.isAlive()) {
		std::cout << "\nyou won (:\n";

		player1.p1.GainEXP(CurrentMonster->xpworth);
		std::cout << "xp gained: " << CurrentMonster->xpworth << std::endl;
		
		//drops a random item
		Item* item_dropped = dropRandomItem();
		if (item_dropped) {
			ItemManager::MoveToBackpack(item_dropped, &player1.p1);
			std::cout << "While rummaging through the monster's corpse, you have found a new item: " << item_dropped->GetData()->Name << std::endl;
		}

		monstersDefeated++;
		create_monster(CurrentMonster, &player1);	
	}
	else {
		std::cout << "\nSorry loser";
	}

	std::cin.ignore(100, '\n');
	std::cout << "\nPress enter to continue\n";
	char a = std::cin.get();
}

void movePlayer(Player& player1) {
	if (player1.xpos == player1.prev_xpos && player1.ypos == player1.prev_ypos)
		return;

	if (theMap[player1.xpos][player1.ypos] == 'M') {
		//if I want to add multiple monsters to a fight, it would need to be sent through a reference using a dereferenced CurrentMonster.
		enterFightSequence(player1);
	}


	//checking if player hasn't moved into a wall
	if (theMap[player1.xpos][player1.ypos] != 'x') {
		//draw character at new location, make old location a blank area
		theMap[player1.xpos][player1.ypos] = 'P';
		theMap[player1.prev_xpos][player1.prev_ypos] = ' ';

		//update previous location
		player1.prev_xpos = player1.xpos;
		player1.prev_ypos = player1.ypos;
	}
	else {
		player1.xpos = player1.prev_xpos;
		player1.ypos = player1.prev_ypos;
	}
}

int main(int argc, char** argv) {
	std::string playerName;
	std::cout << "Insert your name: ";
	std::cin >> playerName;
	system("CLS");

	if (isIt(playerName)) {
		std::cout << "Oh, you're actually here. I hope you're doing fine."
			<< "\nBerserker class unlocked."
			<< "\npress enter to continue.";
		std::cin.ignore(100, '\n');
		char c = getchar();

		system("CLS");
	}

	if (isIt(playerName)) {
		std::cout << "\nChoose a class: \n"
			<< "1: Warrior\n"
			<< "2: Rogue\n"
			<< "3: Wizard\n"
			<< "4: Cleric\n"
			<< "5: Berserker\n";

		int choice = 0;

		while (choice < 1 || choice > 5) {
			std::cin >> choice;
		}
		Item* starterWeapon;
		switch (choice) {
		case 1:
			MainCharacter = new Player(new Warrior());
			MainCharacter->name = playerName;

			starterWeapon = ItemManager::CreateWeapon("Starter Sword", CoreStats(0, 0, 0, 0, 0), WEAPONSLOT::MELEE, 3, 4);
			ItemManager::MoveToBackpack(starterWeapon, &MainCharacter->p1);
			break;
		case 2:
			MainCharacter = new Player(new Rogue());
			MainCharacter->name = playerName;

			starterWeapon = ItemManager::CreateWeapon("Starter Dagger", CoreStats(0, 0, 0, 0, 0), WEAPONSLOT::MELEE, 2, 4);
			ItemManager::MoveToBackpack(starterWeapon, &MainCharacter->p1);
			break;
		case 3:
			MainCharacter = new Player(new Wizard());
			MainCharacter->name = playerName;

			starterWeapon = ItemManager::CreateWeapon("Starter Staff", CoreStats(0, 2, 0, 0, 0), WEAPONSLOT::RANGED, 3, 3);
			ItemManager::MoveToBackpack(starterWeapon, &MainCharacter->p1);
			break;
		case 4:
			MainCharacter = new Player(new Cleric());
			MainCharacter->name = playerName;

			starterWeapon = ItemManager::CreateWeapon("Starter Mace", CoreStats(0, 0, 0, 0, 0), WEAPONSLOT::MELEE, 4, 5);
			ItemManager::MoveToBackpack(starterWeapon, &MainCharacter->p1);
			break;
		case 5:
			MainCharacter = new Player(new Berserker());
			MainCharacter->name = playerName;

			starterWeapon = ItemManager::CreateWeapon("Starter Claymore", CoreStats(2, 0, 1, 0, 0), WEAPONSLOT::MELEE, 4, 5, true);
			ItemManager::MoveToBackpack(starterWeapon, &MainCharacter->p1);
			break;
		default:
			return -1; //failed to make playerCharacter
		}
	}
	else {
		std::cout << "\nChoose a class: \n"
			<< "1: Warrior\n"
			<< "2: Rogue\n"
			<< "3: Wizard\n"
			<< "4: Cleric\n"
			<< "5: ????????\n";

		int choice = 0;

		while (choice < 1 || choice > 4) {
			std::cin >> choice;
		}
		Item* starterWeapon;
		switch (choice) {
		case 1:
			MainCharacter = new Player(new Warrior());
			MainCharacter->name = playerName;

			starterWeapon = ItemManager::CreateWeapon("Starter Sword", CoreStats(0, 0, 0, 0, 0), WEAPONSLOT::MELEE, 3, 4);
			ItemManager::MoveToBackpack(starterWeapon, &MainCharacter->p1);
			break;
		case 2:
			MainCharacter = new Player(new Rogue());
			MainCharacter->name = playerName;

			starterWeapon = ItemManager::CreateWeapon("Starter Dagger", CoreStats(0, 0, 0, 0, 0), WEAPONSLOT::MELEE, 2, 4);
			ItemManager::MoveToBackpack(starterWeapon, &MainCharacter->p1);
			break;
		case 3:
			MainCharacter = new Player(new Wizard());
			MainCharacter->name = playerName;

			starterWeapon = ItemManager::CreateWeapon("Starter Staff", CoreStats(0, 2, 0, 0, 0), WEAPONSLOT::RANGED, 3, 3);
			ItemManager::MoveToBackpack(starterWeapon, &MainCharacter->p1);
			break;
		case 4:
			MainCharacter = new Player(new Cleric());
			MainCharacter->name = playerName;

			starterWeapon = ItemManager::CreateWeapon("Starter Mace", CoreStats(0, 0, 0, 0, 0), WEAPONSLOT::MELEE, 4, 5);
			ItemManager::MoveToBackpack(starterWeapon, &MainCharacter->p1);
			break;
		case 5:
			MainCharacter = new Player(new Berserker());
			MainCharacter->name = playerName;

			starterWeapon = ItemManager::CreateWeapon("Starter Claymore", CoreStats(2, 0, 1, 0, 0), WEAPONSLOT::MELEE, 4, 5, true);
			ItemManager::MoveToBackpack(starterWeapon, &MainCharacter->p1);
			break;
		default:
			return -1; //failed to make playerCharacter
		}
	}

	create_monster(CurrentMonster, MainCharacter);

	theMap[MainCharacter->xpos][MainCharacter->ypos] = 'P';
	theMap[CurrentMonster->xpos][CurrentMonster->ypos] = 'M';

	showMap();

	while (1) {
		std::cout << "\nYou can move using WASD, press I to open inventory, press C to see character sheet:  \n";
		char c = getchar();

		switch (c) {
		case 'w':
			MainCharacter->xpos--;
			break;
		case 's':
			MainCharacter->xpos++;
			break;
		case 'a':
			MainCharacter->ypos--;
			break;
		case 'd':
			MainCharacter->ypos++;
			break;
		case 'i':
			openInventory();
			break;
		case 'c':
			displayCharacterSheet();
			break;
		default:
			break;
		}

		movePlayer(*MainCharacter);

		if (MainCharacter->isAlive()) {
			showMap();
		}
		else {
			break;
		}
	}
	std::cout << "Total monsters defeated: " << monstersDefeated << std::endl;
	std::cout << "Press Enter to Continue." << std::endl;
	char c = getchar();

	return EXIT_SUCCESS;
}