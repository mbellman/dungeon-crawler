#include <Scenes/GameScene.h>
#include <Level/LevelLayout.h>
#include <Level/BlockBuilder.h>
#include <Level/LevelLoader.h>
#include <Entities/CastLight.h>
#include <Entities/TextBox.h>
#include <SoftEngine.h>
#include <MathUtils.h>
#include <GameConstants.h>
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
	if (inputManager->isKeyPressed(Soft::Keys::W)) {
		move(getYawDirection(camera->yaw));
	} else if (inputManager->isKeyPressed(Soft::Keys::A)) {
		move(getYawDirection(camera->yaw + MathUtils::DEG_90));
	} else if (inputManager->isKeyPressed(Soft::Keys::S)) {
		move(getYawDirection(camera->yaw + MathUtils::DEG_180));
	} else if (inputManager->isKeyPressed(Soft::Keys::D)) {
		move(getYawDirection(camera->yaw + MathUtils::DEG_270));
	}

	if (isPlayerMoving() && shouldBobCamera) {
		bobCamera();
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

void GameScene::bobCamera() {
	using namespace GameConstants;

	float yBasis = currentGridPosition.layer * TILE_SIZE - HALF_TILE_SIZE;
	float movementProgress = (float)(getRunningTime() - lastMovementTime) / MOVE_STEP_DURATION;
	float yOffset = -4.0f * sinf(movementProgress * M_PI);

	camera->position.y = yBasis + yOffset;
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
	castLight->lifetime = GameConstants::CAST_LIGHT_LIFETIME;

	add(castLight);

	lastLightCastTime = getRunningTime();
}

Soft::TextureBuffer* GameScene::getBlockTexture(int blockType) {
	using namespace GameConstants;

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

	float progress = (float)(getRunningTime() - lastLightCastTime) / GameConstants::CAST_LIGHT_COOLDOWN_TIME;

	return progress > 1.0f ? 1.0f : progress;
}

GridPosition GameScene::getDirectionalGridPosition(MathUtils::Direction direction) {
	GridPosition targetGridPosition = currentGridPosition;

	switch (direction) {
		case MathUtils::Direction::FORWARD:
			targetGridPosition.z--;
			break;
		case MathUtils::Direction::BACKWARD:
			targetGridPosition.z++;
			break;
		case MathUtils::Direction::LEFT:
			targetGridPosition.x--;
			break;
		case MathUtils::Direction::RIGHT:
			targetGridPosition.x++;
			break;
	}

	return targetGridPosition;
}

Soft::Vec3 GameScene::getGridPositionVec3(GridPosition position) {
	using namespace GameConstants;

	return {
		position.x * TILE_SIZE,
		position.layer * TILE_SIZE - HALF_TILE_SIZE,
		-position.z * TILE_SIZE
	};
}

MathUtils::Direction GameScene::getYawDirection(float yaw) {
	using namespace MathUtils;

	float wrappedYaw = modf(yaw, DEG_360);

	if (wrappedYaw < DEG_45 || wrappedYaw >= DEG_315) {
		return Direction::FORWARD;
	} else if (wrappedYaw < DEG_135 && wrappedYaw >= DEG_45) {
		return Direction::LEFT;
	} else if (wrappedYaw < DEG_225 && wrappedYaw >= DEG_135) {
		return Direction::BACKWARD;
	} else if (wrappedYaw < DEG_315 && wrappedYaw >= DEG_225) {
		return Direction::RIGHT;
	}

	return Direction::UP;
}

bool GameScene::isPlayerMoving() {
	return camera->isTweening();
}

bool GameScene::isPlayerOnStaircase() {
	return levelLayout->isStaircase(currentGridPosition.layer, currentGridPosition.x, currentGridPosition.z);
}

bool GameScene::isWalkablePosition(GridPosition position) {
	return (
		levelLayout->getBlockType(position.layer, position.x, position.z) == GameConstants::BlockTypes::BRIDGE ||
		levelLayout->isEmptyBlock(position.layer, position.x, position.z) &&
		levelLayout->isWalkableBlock(position.layer - 1, position.x, position.z)
	);
}

bool GameScene::isWalkableDownStaircase(MathUtils::Direction direction) {
	using namespace GameConstants;
	using namespace MathUtils;

	GridPosition targetGridPosition = getDirectionalGridPosition(direction);
	int floorBlockType = levelLayout->getBlockType(targetGridPosition.layer - 1, targetGridPosition.x, targetGridPosition.z);

	return (
		(direction == Direction::FORWARD && floorBlockType == BlockTypes::STAIRCASE_BACKWARD) ||
		(direction == Direction::BACKWARD && floorBlockType == BlockTypes::STAIRCASE_FORWARD) ||
		(direction == Direction::LEFT && floorBlockType == BlockTypes::STAIRCASE_RIGHT) ||
		(direction == Direction::RIGHT && floorBlockType == BlockTypes::STAIRCASE_LEFT)
	);
}

bool GameScene::isWalkableUpStaircase(MathUtils::Direction direction) {
	using namespace GameConstants;
	using namespace MathUtils;

	GridPosition targetGridPosition = getDirectionalGridPosition(direction);
	int blockType = levelLayout->getBlockType(targetGridPosition.layer, targetGridPosition.x, targetGridPosition.z);

	return (
		(direction == Direction::FORWARD && blockType == BlockTypes::STAIRCASE_FORWARD) ||
		(direction == Direction::BACKWARD && blockType == BlockTypes::STAIRCASE_BACKWARD) ||
		(direction == Direction::LEFT && blockType == BlockTypes::STAIRCASE_LEFT) ||
		(direction == Direction::RIGHT && blockType == BlockTypes::STAIRCASE_RIGHT)
	);
}

void GameScene::loadLevel() {
	using namespace GameConstants;

	LevelLoader levelLoader("./Assets/Gate1/Level.egp");

	const LevelData& levelData = levelLoader.getLevelData();
	const Soft::Area& size = levelData.size;
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

		light->position = {
			staticLight.position.x * TILE_SIZE,
			staticLight.position.layer * TILE_SIZE - HALF_TILE_SIZE,
			-staticLight.position.z * TILE_SIZE
		};

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

	spawnPlayer(levelData.spawnPosition);

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
	int baseHeight = windowHeight - (windowHeight * GameConstants::RASTER_REGION.height / 100.0f);
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

void GameScene::move(MathUtils::Direction direction) {
	using namespace MathUtils;

	if (isPlayerMoving()) {
		return;
	}

	GridPosition targetGridPosition = getDirectionalGridPosition(direction);
	shouldBobCamera = false;
	lastMovementTime = getRunningTime();

	if (isPlayerOnStaircase()) {
		moveOffStaircase(direction);
	} else if (isWalkableUpStaircase(direction)) {
		moveUpOntoStaircase(direction);
	} else if (isWalkableDownStaircase(direction)) {
		moveDownOntoStaircase(direction);
	} else if (isWalkablePosition(targetGridPosition)) {
		camera->tweenTo(getGridPositionVec3(targetGridPosition), GameConstants::MOVE_STEP_DURATION, Soft::Ease::linear);
		currentGridPosition = targetGridPosition;
		shouldBobCamera = true;
	}
}

void GameScene::moveOffStaircase(MathUtils::Direction direction) {
	using namespace GameConstants;
	using namespace MathUtils;

	int blockType = levelLayout->getBlockType(currentGridPosition.layer, currentGridPosition.x, currentGridPosition.z);
	GridPosition targetGridPosition = currentGridPosition;

	switch (blockType) {
		case BlockTypes::STAIRCASE_FORWARD:
			if (direction == Direction::FORWARD) {
				targetGridPosition.z--;
				targetGridPosition.layer++;
			} else if (direction == Direction::BACKWARD) {
				targetGridPosition.z++;
			}
			break;
		case BlockTypes::STAIRCASE_BACKWARD:
			if (direction == Direction::BACKWARD) {
				targetGridPosition.z++;
				targetGridPosition.layer++;
			} else if (direction == Direction::FORWARD) {
				targetGridPosition.z--;
			}
			break;
		case BlockTypes::STAIRCASE_LEFT:
			if (direction == Direction::LEFT) {
				targetGridPosition.x--;
				targetGridPosition.layer++;
			} else if (direction == Direction::RIGHT) {
				targetGridPosition.x++;
			}
			break;
		case BlockTypes::STAIRCASE_RIGHT:
			if (direction == Direction::RIGHT) {
				targetGridPosition.x++;
				targetGridPosition.layer++;
			} else if (direction == Direction::LEFT) {
				targetGridPosition.x--;
			}
			break;
	}

	Soft::Vec3 targetCameraPosition = getGridPositionVec3(targetGridPosition);

	if (levelLayout->isStaircase(targetGridPosition.layer, targetGridPosition.x, targetGridPosition.z)) {
		targetCameraPosition.y += GameConstants::HALF_TILE_SIZE;
	} else if (levelLayout->isStaircase(targetGridPosition.layer - 1, targetGridPosition.x, targetGridPosition.z)) {
		targetCameraPosition.y -= GameConstants::HALF_TILE_SIZE;
		targetGridPosition.layer--;
	} else if (targetGridPosition == currentGridPosition || !isWalkablePosition(targetGridPosition)) {
		return;
	}

	camera->tweenTo(targetCameraPosition, GameConstants::MOVE_STEP_DURATION, Soft::Ease::linear);
	currentGridPosition = targetGridPosition;
}

void GameScene::moveDownOntoStaircase(MathUtils::Direction direction) {
	using namespace GameConstants;
	using namespace MathUtils;

	GridPosition targetGridPosition = getDirectionalGridPosition(direction);
	targetGridPosition.layer--;

	Soft::Vec3 targetPosition = getGridPositionVec3(targetGridPosition);
	targetPosition.y += HALF_TILE_SIZE;

	int blockType = levelLayout->getBlockType(targetGridPosition.layer, targetGridPosition.x, targetGridPosition.z);

	camera->tweenTo(targetPosition, MOVE_STEP_DURATION, Soft::Ease::linear);
	currentGridPosition = targetGridPosition;
}

void GameScene::moveUpOntoStaircase(MathUtils::Direction direction) {
	using namespace GameConstants;
	using namespace MathUtils;

	GridPosition targetGridPosition = getDirectionalGridPosition(direction);
	Soft::Vec3 targetPosition = getGridPositionVec3(targetGridPosition);
	int blockType = levelLayout->getBlockType(targetGridPosition.layer, targetGridPosition.x, targetGridPosition.z);

	targetPosition.y += HALF_TILE_SIZE;

	camera->tweenTo(targetPosition, MOVE_STEP_DURATION, Soft::Ease::linear);
	currentGridPosition = targetGridPosition;
}

void GameScene::showText(const char* value) {
	TextBox* textBox = (TextBox*)getEntity("textBox");

	textBox->write(value, TextSpeed::NORMAL);
	textBox->show();
}

void GameScene::spawnPlayer(const SpawnPosition& spawnPosition) {
	using namespace GameConstants;
	using namespace MathUtils;

	currentGridPosition.layer = spawnPosition.layer;
	currentGridPosition.x = spawnPosition.x;
	currentGridPosition.z = spawnPosition.z;

	camera->position = getGridPositionVec3(currentGridPosition);

	switch (spawnPosition.direction) {
		case Direction::FORWARD:
			camera->yaw = 0.0f;
			break;
		case Direction::LEFT:
			camera->yaw = DEG_90;
			break;
		case Direction::BACKWARD:
			camera->yaw = DEG_180;
			break;
		case Direction::RIGHT:
			camera->yaw = DEG_270;
			break;
	}
}

void GameScene::updateUI(int dt) {
	Soft::UIObject* lightBar = ui->get("lightBar");

	lightBar->clip(lightBar->getWidth() * getCastLightCooldownProgress(), lightBar->getHeight());
}
