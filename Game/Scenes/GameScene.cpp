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

	Soft::Light* light = new Soft::Light();

	light->range = 750.0f;
	light->setColor({ 255, 150, 50 });
	light->lockTo(camera);
	light->power = 3.0f;

	add(light);

	loadLevel();
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
		return UP;
	} else if (wrappedYaw < DEG_135 && wrappedYaw >= DEG_45) {
		return LEFT;
	} else if (wrappedYaw < DEG_225 && wrappedYaw >= DEG_135) {
		return DOWN;
	} else if (wrappedYaw < DEG_315 && wrappedYaw >= DEG_225) {
		return RIGHT;
	}

	return UP;
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
				int blockType = floorPlan.blockTypes[layer][z][x];

				levelLayout->setBlockType(layer, x, z, blockType);

				if (blockType != BlockTypes::EMPTY) {
					Soft::Object* blockObject = BlockBuilder::createBlockObject(blockType);

					blockObject->position.x += x * TILE_SIZE - HALF_TILE_SIZE;
					blockObject->position.y += layer * TILE_SIZE;
					blockObject->position.z += -z * TILE_SIZE - HALF_TILE_SIZE;

					blockObject->setTexture(getTexture("surface_1"));

					add(blockObject);
				}
			}
		}
	}

	camera->fov = 110;

	settings.ambientLightFactor = 0.0f;
	settings.brightness = 0.1f;
	settings.controlMode = Soft::ControlMode::MOUSE;
}

void GameScene::move(MathUtils::Direction direction) {
	using namespace MathUtils;

	if (isMoving()) {
		return;
	}

	Soft::Vec3 movementTarget = camera->position;

	switch (direction) {
		case UP:
			movementTarget.z += GameConstants::TILE_SIZE;
			break;
		case DOWN:
			movementTarget.z -= GameConstants::TILE_SIZE;
			break;
		case LEFT:
			movementTarget.x -= GameConstants::TILE_SIZE;
			break;
		case RIGHT:
			movementTarget.x += GameConstants::TILE_SIZE;
			break;
	}

	camera->tweenTo(movementTarget, GameConstants::MOVE_STEP_DURATION, Soft::Ease::linear);
}
