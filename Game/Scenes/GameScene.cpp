#include <Scenes/GameScene.h>
#include <Level/LevelLayout.h>
#include <Level/BlockBuilder.h>
#include <Level/LevelLoader.h>
#include <Entities/Player.h>
#include <Entities/CastLight.h>
#include <Entities/TextBox.h>
#include <SoftEngine.h>
#include <MathUtils.h>
#include <GameUtils.h>
#include <cmath>
#include <math.h>
#include <SDL_ttf.h>

/**
 * GameScene
 * ---------
 */
GameScene::GameScene() {
	uiFont = TTF_OpenFont("./Assets/Fonts/FreeMono.ttf", 20);
}

GameScene::~GameScene() {
	if (levelLayout != nullptr) {
		delete levelLayout;
	}

	TTF_CloseFont(uiFont);
}

void GameScene::load() {
	loadTextures();
	loadLevel();
	loadUI();

	addPlayer();
	addCameraLight();

	inputManager->onMouseUp([=]() {
		castLight();
	});

	inputManager->onKeyDown([=](const SDL_Keycode& code) {
		if (code == SDLK_SPACE) {
			TextBox* textBox = (TextBox*)getEntity("textBox");

			if (textBox->isWriting()) {
				textBox->skipWritingAnimation();
			} else if (textBox->isShown()) {
				textBox->hide();
			}
		}
	});

	camera->fov = 110;
	settings.controlMode = Soft::ControlMode::MOUSE;
}

void GameScene::onUpdate(int dt) {
	using namespace GameUtils;

	Player* player = (Player*)getEntity("player");

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

void GameScene::addCameraLight() {
	Soft::Light* light = new Soft::Light();

	light->range = 750.0f;
	light->power = 0.5f;
	light->setColor({ 255, 150, 50 });
	light->lockTo(camera);

	add(light);
}

void GameScene::addPlayer() {
	Player* player = new Player(camera, levelLayout);

	player->spawn(playerSpawnPosition);

	add("player", player);
}

void GameScene::castLight() {
	if (getCastLightCooldownProgress() < 1.0f) {
		showText("Hey! Hold on a second.");

		return;
	}

	TextBox* textBox = (TextBox*)getEntity("textBox");

	if (textBox->isShown()) {
		textBox->hide();
	}

	CastLight* castLight = new CastLight(camera->position, camera->getDirection());
	castLight->lifetime = GameUtils::CAST_LIGHT_LIFETIME;

	add(castLight);

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

	for (int i = 0; i < levelData.staticLights.size(); i++) {
		const StaticLight& staticLight = levelData.staticLights[i];
		Soft::Light* light = new Soft::Light();

		light->position = getGridPositionVec3(staticLight.position);
		light->setColor(staticLight.color);
		light->range = staticLight.range;
		light->isStatic = true;

		add(light);
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
	lightBar->setColor({ 0, 255, 0 });
	lightBar->setAlpha(0.5f);
	lightBar->position = { 92, baseY + 27 };

	ui->add("leftColumn", leftColumn);
	ui->add("rightColumn", rightColumn);
	ui->add("base", base);
	ui->add("lightBar", lightBar);
}

void GameScene::showText(const char* value) {
	TextBox* textBox = (TextBox*)getEntity("textBox");

	textBox->write(value, TextSpeed::NORMAL);
	textBox->show();
}

void GameScene::updateUI(int dt) {
	Soft::UIObject* lightBar = ui->get("lightBar");

	lightBar->clip(lightBar->getWidth() * getCastLightCooldownProgress(), lightBar->getHeight());
}
