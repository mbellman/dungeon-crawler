#include <Items.h>
#include <map>

static ItemData potion = {
	ItemType::CONSUMABLE,
	"Potion", 20.0f
};

static ItemData scepter = {
	ItemType::WEAPON,
	"Scepter", 25.0f
};

static ItemData shield = {
	ItemType::ARMOR,
	"Shield", 25.0f
};

std::map<int, ItemData> items = {
	{ Items::POTION, potion },
	{ Items::SCEPTER, scepter },
	{ Items::SHIELD, shield }
};
