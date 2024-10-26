#pragma once

#include "MyRPGLib/playerCharacter.h"
#include "MyRPGLib/playableclasses_fwd.h"
#include "MyRPGLib/monster.h"
#include "MyRPGLib/random.h"
#include "MyRPGLib/item.h"
#include "MyRPGLib/item_manager.h"

#include <iostream>
#include <string>

bool isIt(std::string name) {
	// Converter a string para maiúsculas
	for (int i = 0; i < name.length(); i++) {
		name[i] = toupper(name[i]);
	}

	if (name == "PASSARRINHO") {
		return true;
	}

	return false;
}

char theMap[12][13] =
{ "xxxxxxxxxxxx",
	"x          x",
	"x          x",
	"x          x",
	"x          x",
	"x          x",
	"x          x",
	"x          x",
	"x          x",
	"x          x",
	"x          x",
	"xxxxxxxxxxxx"
};

void showMap() {
	system("CLS");
	for (int i = 0; i < 12; i++) {
		for (int j = 0; j < 13; j++) {
			std::cout << theMap[i][j];
		}
		std::cout << "\n";
	}
}


//enum for facing direction later?
struct Player {
	Player(PlayerCharacterDelegate* charclass) : p1(charclass) {}
	Player() = delete;
	bool isAlive() { return (p1.GetCurrentHP() > 0); }
	PlayerCharacter p1;
	int prev_xpos = 3;
	int prev_ypos = 3;
	int xpos = 3;
	int ypos = 3;
	std::string name;
};

struct Enemy {
	Enemy(int hp, int min, int max) : monster(hp, min, max) {
		xpworth = (hp + min + max) * 2;
	}
	bool isAlive() { return (monster.HP.GetCurrent() > 0); }
	Monster monster;
	int xpos = 3;
	int ypos = 3;
	int xpworth;
};