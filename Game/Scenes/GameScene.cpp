#include <Scenes/GameScene.h>
#include <Level/LevelLayout.h>
#include <Level/BlockBuilder.h>
#include <Level/LevelLoader.h>
#include <Entities/Player.h>
#include <Entities/CastLight.h>
#include <Entities/Staff.h>
#include <Entities/TextBox.h>
#include <Entities/ItemMenu.h>
#include <Entities/HUD.h>
#include <Entities/MiniMap.h>
#include <Entities/Torch.h>
#include <Entities/Chest.h>
#include <Entities/Door.h>
#include <MathUtils.h>
#include <GameUtils.h>
#include <Party.h>
#include <SoftEngine.h>
#include <cmath>
#include <math.h>
#include <string>
#include <SDL_ttf.h>

/**
 * GameScene
 * ---------
 */
GameScene::GameScene() {
	uiFont = TTF_OpenFont("./Assets/Fonts/FreeMono.ttf", 20);
	party = new Party();
	inventory = new Inventory();
}

GameScene::~GameScene() {
	if (levelLayout != nullptr) {
		delete levelLayout;
	}

	TTF_CloseFont(uiFont);

	delete inventory;
	delete party;
}

void GameScene::load() {
	loadTextures();
	loadLevel();
	loadParty();

	add("textBox", new TextBox(uiFont));

	addPlayer();
	addStaff();
	addHUD();

	inputManager->onMouseUp([=]() {
		castLight();
	});

	inputManager->onKeyDown([=](const SDL_Keycode& code) {
		if (isItemMenuOpen()) {
			handleItemMenuKeyDown(code);
		} else {
			handleGameKeyDown(code);
		}
	});

	camera->fov = 110;
	settings.controlMode = Soft::ControlMode::MOUSE;
}

void GameScene::onUpdate(int dt) {
	using namespace GameUtils;

	auto* player = getEntity<Player>("player");

	if (inputManager->isKeyPressed(Soft::Keys::W)) {
		player->move(getYawDirection(camera->yaw));
	} else if (inputManager->isKeyPressed(Soft::Keys::A)) {
		player->move(getYawDirection(camera->yaw + MathUtils::DEG_90));
	} else if (inputManager->isKeyPressed(Soft::Keys::S)) {
		player->move(getYawDirection(camera->yaw + MathUtils::DEG_180));
	} else if (inputManager->isKeyPressed(Soft::Keys::D)) {
		player->move(getYawDirection(camera->yaw + MathUtils::DEG_270));
	}
}

void GameScene::addPlayer() {
	Player* player = new Player(camera, levelLayout);

	player->spawn(playerSpawnPosition);

	add("player", player);
}

void GameScene::addStaff() {
	Staff* staff = new Staff(camera, getTexture("staff"));

	add("staff", staff);
}

void GameScene::castLight() {
	auto* hud = getEntity<HUD>("hud");

	if (hud->getCastLightCooldownProgress() < 1.0f) {
		showText("Hey! Hold on a second.");

		return;
	}

	hud->trackCastLightTime();

	auto* textBox = getEntity<TextBox>("textBox");

	if (textBox->isShown()) {
		textBox->hide();
	}

	CastLight* castLight = new CastLight(camera->position, camera->getDirection(), camera, getTexture("castLight"));
	castLight->lifetime = GameUtils::CAST_LIGHT_LIFETIME;

	add(castLight);

	auto* staff = getEntity<Staff>("staff");

	staff->swing();
	staff->dispelLight();
}

Soft::TextureBuffer* GameScene::getBlockTexture(int blockType) {
	using namespace GameUtils;

	switch (blockType) {
		case BlockTypes::SOLID_1:
			return getTexture("solid_1");
		case BlockTypes::SOLID_2:
			return getTexture("solid_2");
		case BlockTypes::COLUMN_BOTTOM:
		case BlockTypes::COLUMN_MIDDLE:
		case BlockTypes::COLUMN_TOP:
			return getTexture("column");
		case BlockTypes::STAIRCASE_BACKWARD:
		case BlockTypes::STAIRCASE_FORWARD:
		case BlockTypes::STAIRCASE_LEFT:
		case BlockTypes::STAIRCASE_RIGHT:
			return getTexture("staircase");
		case BlockTypes::BRIDGE:
			return getTexture("bridge");
		default:
			return nullptr;
	}
}

void GameScene::handleAction() {
	auto* textBox = getEntity<TextBox>("textBox");

	if (textBox->isWriting()) {
		textBox->skipWritingAnimation();
	} else if (textBox->isShown()) {
		textBox->hide();
	} else {
		auto* player = getEntity<Player>("player");
		GridPosition nextPosition = player->getDirectionalGridPosition(player->getDirection());
		Chest* chest = levelLayout->getMatchingChest(nextPosition);
		Door* door = levelLayout->getMatchingDoor(nextPosition);

		if (chest != nullptr) {
			handleChestAction(chest);
		} else if (door != nullptr) {
			handleDoorAction(door);
		}
	}
}

void GameScene::handleChestAction(Chest* chest) {
	if (!chest->isOpen()) {
		auto* player = getEntity<Player>("player");
		const ItemData& itemData = chest->getItemData();

		chest->open(player->getDirection());
		inventory->addItem(itemData);

		showItemObtainedText(itemData.name);
	}
}

void GameScene::handleDoorAction(Door* door) {
	if (!door->isOpen()) {
		door->open();
	}
}

void GameScene::handleGameKeyDown(const SDL_Keycode& code) {
	switch (code) {
		case SDLK_SPACE:
			handleAction();
			break;
		case SDLK_e:
			showItemMenu();
			break;
	}
}

void GameScene::handleItemMenuKeyDown(const SDL_Keycode& code) {
	auto* itemMenu = getEntity<ItemMenu>("itemMenu");

	switch (code) {
		case SDLK_DOWN:
			itemMenu->scrollDown();
			break;
		case SDLK_UP:
			itemMenu->scrollUp();
			break;
		case SDLK_RETURN:
			itemMenu->confirm();
			break;
		case SDLK_e:
			itemMenu->cancel();
			break;
	}

	if (itemMenu->isClosed()) {
		remove("itemMenu");
	}
}

bool GameScene::isItemMenuOpen() {
	return getEntity<ItemMenu>("itemMenu") != nullptr;
}

void GameScene::addHUD() {
	auto* player = getEntity<Player>("player");

	MiniMap* miniMap = new MiniMap(levelLayout, player);

	add("hud", new HUD({ controller->getWindowWidth(), controller->getWindowHeight() }, party));
	add(miniMap);
}

void GameScene::loadLevel() {
	using namespace GameUtils;

	LevelLoader levelLoader("./Assets/Gate1/Level.egp");

	const LevelData& levelData = levelLoader.getLevelData();
	const Soft::Area& size = levelData.size;

	playerSpawnPosition = levelData.spawnPosition;
	levelLayout = new LevelLayout(levelData.layers.size(), size);

	for (int layer = 0; layer < levelLayout->getTotalLayers(); layer++) {
		for (int z = 0; z < size.height; z++) {
			for (int x = 0; x < size.width; x++) {
				int blockIndex = z * size.width + x;
				int blockType = levelData.layers[layer].blockTypes[blockIndex];

				levelLayout->setBlockType(layer, x, z, blockType);
			}
		}
	}

	for (const auto& torchData : levelData.torches) {
		Torch* torch = new Torch(torchData, getTexture("fireTexture"), camera);

		add(torch);
	}

	for (const auto& chestData : levelData.chests) {
		Chest* chest = new Chest(chestData, getTexture("chestBox"), getTexture("chestLid"));

		levelLayout->addChest(chest);
		add(chest);
	}

	for (const auto& doorData : levelData.doors) {
		Door* door = new Door(doorData, getTexture("doorFrame"), getTexture("doorSlab"));

		levelLayout->addDoor(door);
		add(door);
	}

	BlockBuilder builder(levelLayout);

	while (builder.hasBlocksRemaining()) {
		Block block = builder.getNextBlock();

		if (block.object != nullptr) {
			Soft::TextureBuffer* texture = getBlockTexture(block.type);

			if (texture != nullptr) {
				block.object->setTexture(texture);
			}

			add(block.object);
		}
	}

	settings.ambientLightColor = levelData.ambientLightColor;
	settings.ambientLightVector = levelData.ambientLightVector;
	settings.ambientLightFactor = levelData.ambientLightFactor;
	settings.visibility = levelData.visibility;
	settings.brightness = levelData.brightness;
}

void GameScene::loadParty() {
	party->addMember(new PartyMember("Player", 0));
	party->addMember(new PartyMember("Sobek", 1));
	party->addMember(new PartyMember("Khonsu", 2));
	party->addMember(new PartyMember("Player", 3));
}

void GameScene::loadTextures() {
	add("staff", new Soft::TextureBuffer("./Assets/Textures/staff.png"));
	add("solid_1", new Soft::TextureBuffer("./Assets/Textures/Blocks/solid_1.png"));
	add("solid_2", new Soft::TextureBuffer("./Assets/Textures/Blocks/solid_2.png"));
	add("column", new Soft::TextureBuffer("./Assets/Textures/Blocks/column.png"));
	add("staircase", new Soft::TextureBuffer("./Assets/Textures/Blocks/staircase.png"));
	add("bridge", new Soft::TextureBuffer("./Assets/Textures/Blocks/bridge.png"));
	add("chestBox", new Soft::TextureBuffer("./Assets/Textures/Chest/box.png"));
	add("chestLid", new Soft::TextureBuffer("./Assets/Textures/Chest/lid.png"));
	add("doorFrame", new Soft::TextureBuffer("./Assets/Textures/Door/frame.png"));
	add("doorSlab", new Soft::TextureBuffer("./Assets/Textures/Door/slab.png"));

	Soft::TextureBuffer* castLightTexture = new Soft::TextureBuffer("./Assets/Textures/CastLight/blue.png");
	castLightTexture->shouldUseMipmaps = false;

	add("castLight", castLightTexture);

	Soft::TextureBuffer* fireTexture = new Soft::TextureBuffer("./Assets/Textures/Fire/1.png");
	fireTexture->shouldUseMipmaps = false;

	fireTexture->addFrame("./Assets/Textures/Fire/2.png");
	fireTexture->addFrame("./Assets/Textures/Fire/3.png");
	fireTexture->addFrame("./Assets/Textures/Fire/4.png");
	fireTexture->addFrame("./Assets/Textures/Fire/5.png");
	fireTexture->addFrame("./Assets/Textures/Fire/6.png");
	fireTexture->addFrame("./Assets/Textures/Fire/7.png");
	fireTexture->addFrame("./Assets/Textures/Fire/8.png");

	add("fireTexture", fireTexture);
}

void GameScene::showItemMenu() {
	ItemMenu* itemMenu = new ItemMenu(inventory, uiFont);

	add("itemMenu", itemMenu);
}

void GameScene::showItemObtainedText(const char* itemName) {
	char message[100];

	sprintf(message, "Obtained %s!", itemName);
	showText(message);
}

void GameScene::showText(const char* value) {
	auto* textBox = getEntity<TextBox>("textBox");

	textBox->write(value, TextSpeed::NORMAL);
	textBox->show();
}
