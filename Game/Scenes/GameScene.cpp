#include <Scenes/GameScene.h>
#include <Level/LevelLayout.h>
#include <Level/BlockBuilder.h>
#include <Level/Floors.h>
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

	inputManager->onMouseClick([=]() {
		const Soft::Vec3& cameraDirection = camera->getDirection();

		Soft::Light* light = new Soft::Light();

		light->setColor({ 0, 255, 0 });
		light->lifetime = 5000;
		light->position = camera->position + cameraDirection * 100.0f;

		light->onUpdate = [=](int dt) {
			light->position += cameraDirection * 25.0f;
		};

		add(light);
	});

	camera->fov = 110;

	settings.ambientLightFactor = 0.0f;
	settings.brightness = 0.05f;
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
	using namespace Floors;

	FloorPlan floorPlan = Floor1;

	levelLayout = new LevelLayout(floorPlan.totalLayers, floorPlan.size);
	const Soft::Area& size = levelLayout->getSize();

	for (int layer = 0; layer < levelLayout->getTotalLayers(); layer++) {
		for (int z = 0; z < size.height; z++) {
			for (int x = 0; x < size.width; x++) {
				int blockType = floorPlan.blocks[layer][z][x];

				levelLayout->setBlockType(layer, x, z, blockType);
			}
		}
	}

	BlockBuilder builder(levelLayout);

	while (!builder.isComplete()) {
		Soft::Object* blockObject = builder.getNextBlockObject();

		if (blockObject != nullptr) {
			blockObject->setTexture(getTexture("surface_1"));

			add(blockObject);
		}
	}

	spawn(floorPlan.spawnPosition);
}

void GameScene::move(MathUtils::Direction direction) {
	using namespace MathUtils;

	if (isMoving()) {
		return;
	}

	Soft::Vec3 movementTarget = camera->position;

	switch (direction) {
		case Direction::FORWARD:
			movementTarget.z += GameConstants::TILE_SIZE;
			break;
		case Direction::BACKWARD:
			movementTarget.z -= GameConstants::TILE_SIZE;
			break;
		case Direction::LEFT:
			movementTarget.x -= GameConstants::TILE_SIZE;
			break;
		case Direction::RIGHT:
			movementTarget.x += GameConstants::TILE_SIZE;
			break;
	}

	camera->tweenTo(movementTarget, GameConstants::MOVE_STEP_DURATION, Soft::Ease::linear);
}

void GameScene::spawn(SpawnPosition position) {
	using namespace GameConstants;
	using namespace MathUtils;

	camera->position = {
		position.x * TILE_SIZE,
		position.layerIndex * TILE_SIZE - HALF_TILE_SIZE,
		-position.z * TILE_SIZE
	};

	switch (position.direction) {
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
