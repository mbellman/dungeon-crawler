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
	add("surface_1", new Soft::TextureBuffer("./Assets/Gate1/surface_1.png"));
	loadLevel();

	Soft::Light* light = new Soft::Light();

	light->range = 750.0f;
	light->setColor({ 255, 150, 50 });
	light->lockTo(camera);
	light->power = 3.0f;

	add(light);

	inputManager->onMouseUp([=]() {
		const Soft::Vec3& cameraDirection = camera->getDirection();

		Soft::Light* light = new Soft::Light();

		light->setColor({ 0, 255, 0 });
		light->lifetime = 5000;
		light->position = camera->position + cameraDirection * 100.0f;
		light->range = 1000.0f;

		light->onUpdate = [=](int dt) {
			light->position += cameraDirection * (float)dt;
		};

		add(light);
	});

	camera->fov = 110;

	settings.ambientLightFactor = 0.0f;
	settings.brightness = 0.1f;
	settings.visibility = 5000;
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

	while (!builder.isComplete()) {
		Soft::Object* blockObject = builder.getNextBlockObject();

		if (blockObject != nullptr) {
			blockObject->setTexture(getTexture("surface_1"));

			add(blockObject);
		}
	}

	spawn(levelData.spawnPosition);
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
