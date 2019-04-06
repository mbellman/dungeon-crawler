#pragma once

#include <map>
#include <string>

enum ItemType {
	WEAPON,
	ARMOR,
	CONSUMABLE
};

enum Items {
	POTION = 0,
	SCEPTER = 1,
	SHIELD = 2
};

struct ItemData {
	ItemType type;
	const char* name;
	float value;
};

extern std::map<int, ItemData> ItemMap;
