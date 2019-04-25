#include <Scenes/BattleScene.h>
#include <Party.h>
#include <Inventory.h>
#include <UI/BattleMenu.h>

/**
 * BattleScene
 * -----------
 */
BattleScene::BattleScene(Party* party, Inventory* inventory) {
	this->party = party;
	this->inventory = inventory;
}

void BattleScene::load() {
	addMenu();

	settings.controlMode = 0;
}

void BattleScene::addMenu() {
	BattleMenu* battleMenu = new BattleMenu(party, inventory);

	add("battleMenu", battleMenu);
}
