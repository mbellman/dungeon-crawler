#include <Scenes/GameScene.h>
#include <Level/LevelLayout.h>
#include <Level/BlockBuilder.h>
#include <Level/LevelLoader.h>
#include <Entities/Player.h>
#include <Entities/CastLight.h>
#include <Entities/Chest.h>
#include <Entities/Staff.h>
#include <Entities/TextBox.h>
#include <Entities/ItemMenu.h>
#include <Entities/Torch.h>
#include <SoftEngine.h>
#include <MathUtils.h>
#include <GameUtils.h>
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
	inventory = new Inventory();
}

GameScene::~GameScene() {
	if (levelLayout != nullptr) {
		delete levelLayout;
	}

	TTF_CloseFont(uiFont);

	delete inventory;
}

void GameScene::load() {
	loadTextures();
	loadLevel();
	loadUI();

	addPlayer();
	addStaff();

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

	updateUI(dt);
}

void GameScene::addPlayer() {
	Player* player = new Player(camera, levelLayout);

	player->spawn(playerSpawnPosition);

	add("player", player);
}

void GameScene::addStaff() {
	Staff* staff = new Staff(camera);

	add("staff", staff);
}

void GameScene::castLight() {
	if (getCastLightCooldownProgress() < 1.0f) {
		showText("Hey! Hold on a second.");

		return;
	}

	auto* textBox = getEntity<TextBox>("textBox");

	if (textBox->isShown()) {
		textBox->hide();
	}

	CastLight* castLight = new CastLight(camera->position, camera->getDirection(), camera, getTexture("cast_light"));
	castLight->lifetime = GameUtils::CAST_LIGHT_LIFETIME;

	add(castLight);

	auto* staff = getEntity<Staff>("staff");

	staff->swing();
	staff->dispelLight();

	lastLightCastTime = getRunningTime();
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

float GameScene::getCastLightCooldownProgress() {
	if (lastLightCastTime == 0) {
		return 1.0f;
	}

	float progress = (float)(getRunningTime() - lastLightCastTime) / GameUtils::CAST_LIGHT_COOLDOWN_TIME;

	return progress > 1.0f ? 1.0f : progress;
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

		if (chest != nullptr) {
			handleChestAction(chest);
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
		Chest* chest = new Chest(chestData, getTexture("chest_box"), getTexture("chest_lid"));

		levelLayout->addChest(chest);
		add(chest);
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

void GameScene::loadTextures() {
	add("solid_1", new Soft::TextureBuffer("./Assets/BlockTextures/solid_1.png"));
	add("solid_2", new Soft::TextureBuffer("./Assets/BlockTextures/solid_2.png"));
	add("column", new Soft::TextureBuffer("./Assets/BlockTextures/column.png"));
	add("staircase", new Soft::TextureBuffer("./Assets/BlockTextures/staircase.png"));
	add("bridge", new Soft::TextureBuffer("./Assets/BlockTextures/bridge.png"));
	add("chest_box", new Soft::TextureBuffer("./Assets/BlockTextures/Chest/box.png"));
	add("chest_lid", new Soft::TextureBuffer("./Assets/BlockTextures/Chest/lid.png"));

	Soft::TextureBuffer* castLightTexture = new Soft::TextureBuffer("./Assets/BlockTextures/CastLight/blue.png");
	castLightTexture->shouldUseMipmaps = false;

	add("cast_light", castLightTexture);

	Soft::TextureBuffer* fireTexture = new Soft::TextureBuffer("./Assets/BlockTextures/Fire/1.png");
	fireTexture->shouldUseMipmaps = false;

	fireTexture->addFrame("./Assets/BlockTextures/Fire/2.png");
	fireTexture->addFrame("./Assets/BlockTextures/Fire/3.png");
	fireTexture->addFrame("./Assets/BlockTextures/Fire/4.png");
	fireTexture->addFrame("./Assets/BlockTextures/Fire/5.png");
	fireTexture->addFrame("./Assets/BlockTextures/Fire/6.png");
	fireTexture->addFrame("./Assets/BlockTextures/Fire/7.png");
	fireTexture->addFrame("./Assets/BlockTextures/Fire/8.png");

	add("fireTexture", fireTexture);
}

void GameScene::loadUI() {
	int windowWidth = controller->getWindowWidth();
	int windowHeight = controller->getWindowHeight();
	int baseHeight = windowHeight - (windowHeight * GameUtils::RASTER_REGION.height / 100.0f);
	int baseY = windowHeight - baseHeight;

	add("textBox", new TextBox(uiFont));

	Soft::UIGraphic* leftColumn = new Soft::UIGraphic("./Assets/UI/column.png");
	leftColumn->position = { -30, 0 };

	Soft::UIGraphic* rightColumn = new Soft::UIGraphic("./Assets/UI/column.png");
	rightColumn->position = { windowWidth - rightColumn->getWidth() + 30, 0 };

	Soft::UIGraphic* base = new Soft::UIGraphic("./Assets/UI/hud.png");
	base->position = { 0, baseY };

	Soft::UIRect* lightBar = new Soft::UIRect();
	lightBar->setSize(200, 24);
	lightBar->setColor({ 0, 0, 255 });
	lightBar->setAlpha(0.5f);
	lightBar->position = { 92, baseY + 27 };

	ui->add(leftColumn);
	ui->add(rightColumn);
	ui->add(base);
	ui->add("lightBar", lightBar);
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

void GameScene::updateUI(int dt) {
	Soft::UIObject* lightBar = ui->get("lightBar");

	lightBar->clip(lightBar->getWidth() * getCastLightCooldownProgress(), lightBar->getHeight());
}
