#include <Scenes/GameScene.h>
#include <Level/LevelLayout.h>
#include <Level/BlockBuilder.h>
#include <Level/LevelLoader.h>
#include <SoftEngine.h>
#include <MathUtils.h>
#include <GameConstants.h>
#include <cmath>
#include <math.h>

/**
 * GameScene
 * ---------
 */
GameScene::~GameScene() {
	if (levelLayout != nullptr) {
		delete levelLayout;
	}
}

void GameScene::load() {
	add("solid_1", new Soft::TextureBuffer("./Assets/BlockTextures/solid_1.png"));
	add("solid_2", new Soft::TextureBuffer("./Assets/BlockTextures/solid_2.png"));

	loadLevel();
	loadUI();
	addCameraLight();

	inputManager->onMouseUp([=]() {
		castLight();
	});

	camera->fov = 110;
	settings.ambientLightFactor = 0.0f;
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

bool GameScene::canMoveInDirection(MathUtils::Direction direction) {
	if (levelLayout == nullptr) {
		return false;
	}

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

	return (
		levelLayout->isEmptyBlock(targetGridPosition.layer, targetGridPosition.x, targetGridPosition.z) &&
		levelLayout->isWalkableBlock(targetGridPosition.layer - 1, targetGridPosition.x, targetGridPosition.z)
	);
}

void GameScene::castLight() {
	using namespace GameConstants;

	if (getCastLightCooldownProgress() < 1.0f) {
		return;
	}

	const Soft::Vec3& cameraDirection = camera->getDirection();
	Soft::Light* light = new Soft::Light();

	light->setColor({ 0, 255, 0 });
	light->lifetime = CAST_LIGHT_LIFETIME;
	light->position = camera->position + cameraDirection * 100.0f;
	light->range = CAST_LIGHT_RANGE;

	light->onUpdate = [=](int dt) {
		light->position += cameraDirection * (float)dt * 1.5f;
	};

	Soft::Cube* lightCube = new Soft::Cube(3.0f);

	lightCube->setColor({ 0, 255, 0 });
	lightCube->lifetime = CAST_LIGHT_LIFETIME;
	lightCube->hasLighting = false;
	lightCube->lockTo(light);

	add(light);
	add(lightCube);

	lastLightCastTime = getRunningTime();
}

Soft::TextureBuffer* GameScene::getBlockTexture(int blockType) {
	using namespace GameConstants;

	switch (blockType) {
		case BlockTypes::SOLID_1:
			return getTexture("solid_1");
		case BlockTypes::SOLID_2:
			return getTexture("solid_2");
		default:
			return nullptr;
	}
}

float GameScene::getCastLightCooldownProgress() {
	if (lastLightCastTime == 0) {
		return 1.0f;
	}

	float progress = (float)(getRunningTime() - lastLightCastTime) / GameConstants::CAST_LIGHT_LIFETIME;

	return progress > 1.0f ? 1.0f : progress;
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

bool GameScene::isMoving() {
	return camera->isTweening();
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

	spawn(levelData.spawnPosition);

	settings.visibility = levelData.visibility;
	settings.brightness = levelData.brightness;
}

void GameScene::loadUI() {
	int windowWidth = controller->getWindowWidth();
	int windowHeight = controller->getWindowHeight();
	int baseHeight = windowHeight - (windowHeight * GameConstants::RASTER_REGION.height / 100.0f);
	int baseY = windowHeight - baseHeight;

	Soft::UIGraphic* leftColumn = new Soft::UIGraphic("./Assets/UI/column.png");
	leftColumn->position = { -30, 0 };

	Soft::UIGraphic* rightColumn = new Soft::UIGraphic("./Assets/UI/column.png");
	rightColumn->position = { windowWidth - rightColumn->getWidth() + 30, 0 };

	Soft::UIRect* base = new Soft::UIRect();
	base->setSize(windowWidth, baseHeight);
	base->position = { 0, baseY };
	base->setColor({ 60, 40, 5 });

	Soft::UIRect* lightBar = new Soft::UIRect();
	lightBar->setSize(200, 20);
	lightBar->setColor({ 0, 255, 0 });
	lightBar->position = { 100, baseY + 50 };

	ui->add("leftColumn", leftColumn);
	ui->add("rightColumn", rightColumn);
	ui->add("base", base);
	ui->add("lightBar", lightBar);
}

void GameScene::move(MathUtils::Direction direction) {
	using namespace MathUtils;

	if (isMoving() || !canMoveInDirection(direction)) {
		return;
	}

	Soft::Vec3 movementTarget = camera->position;

	switch (direction) {
		case Direction::FORWARD:
			movementTarget.z += GameConstants::TILE_SIZE;
			currentGridPosition.z--;
			break;
		case Direction::BACKWARD:
			movementTarget.z -= GameConstants::TILE_SIZE;
			currentGridPosition.z++;
			break;
		case Direction::LEFT:
			movementTarget.x -= GameConstants::TILE_SIZE;
			currentGridPosition.x--;
			break;
		case Direction::RIGHT:
			movementTarget.x += GameConstants::TILE_SIZE;
			currentGridPosition.x++;
			break;
	}

	camera->tweenTo(movementTarget, GameConstants::MOVE_STEP_DURATION, Soft::Ease::linear);
}

void GameScene::spawn(const SpawnPosition& spawnPosition) {
	using namespace GameConstants;
	using namespace MathUtils;

	currentGridPosition.layer = spawnPosition.layer;
	currentGridPosition.x = spawnPosition.x;
	currentGridPosition.z = spawnPosition.z;

	camera->position = {
		spawnPosition.x * TILE_SIZE,
		spawnPosition.layer * TILE_SIZE - HALF_TILE_SIZE,
		-spawnPosition.z * TILE_SIZE
	};

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
