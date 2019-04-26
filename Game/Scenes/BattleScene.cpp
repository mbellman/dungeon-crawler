#include <Scenes/BattleScene.h>
#include <Party.h>
#include <Inventory.h>
#include <UI/BattleMenu.h>
#include <SoftEngine.h>
#include <GameUtils.h>

/**
 * BattleScene
 * -----------
 */
BattleScene::BattleScene(Party* party, Inventory* inventory) {
	this->party = party;
	this->inventory = inventory;
}

void BattleScene::load() {
	loadTextures();

	addScenery();
	addMenu();

	settings.controlMode = 0;
	settings.visibility = 1500;
	settings.brightness = 0.5f;
}

void BattleScene::addMenu() {
	BattleMenu* battleMenu = new BattleMenu(party, inventory);

	add("battleMenu", battleMenu);
}

void BattleScene::addScenery() {
	Soft::Mesh* ground = new Soft::Mesh(20, 20, GameUtils::TILE_SIZE);

	ground->position = {
		-(10 * GameUtils::TILE_SIZE),
		0.0f,
		0.0f
	};

	ground->setTexture(getTexture("solid_1"));
	ground->setTextureInterval(1, 1);

	add(ground);

	Soft::UIGraphic* leftColumn = new Soft::UIGraphic("./Assets/UI/column.png");
	leftColumn->position = { -30, 0 };

	Soft::UIGraphic* rightColumn = new Soft::UIGraphic("./Assets/UI/column.png");
	rightColumn->position = { GameUtils::WINDOW_SIZE.width - rightColumn->getWidth() + 30, 0 };

	ui->add(leftColumn);
	ui->add(rightColumn);
}

void BattleScene::loadTextures() {
	add("solid_1", new Soft::TextureBuffer("./Assets/Textures/Blocks/solid_1.png"));
}
