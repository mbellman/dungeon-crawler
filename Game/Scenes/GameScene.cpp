#include <Scenes/GameScene.h>
#include <SoftEngine.h>
#include <MathUtils.h>
#include <cmath>
#include <math.h>

/**
 * GameScene
 * ---------
 */
void GameScene::load() {
	add("ground-tex", new Soft::TextureBuffer("./Assets/gate1/ground.png"));

	Soft::Mesh* ground = new Soft::Mesh(25, 25, GameScene::TILE_SIZE);

	ground->position = { -1250.0f, 0, 0 };
	ground->setTexture(getTexture("ground-tex"));
	ground->setTextureInterval(2, 2);

	add(ground);

	settings.ambientLightFactor = 0.0f;
	settings.brightness = 0.2f;
	settings.controlMode = Soft::ControlMode::MOUSE;
}

void GameScene::onUpdate(int dt) {
	if (inputManager->isKeyPressed(Soft::Keys::W)) {
		move(getYawTileDirection(camera->yaw));
	} else if (inputManager->isKeyPressed(Soft::Keys::A)) {
		move(getYawTileDirection(camera->yaw + MathUtils::DEG_90));
	} else if (inputManager->isKeyPressed(Soft::Keys::S)) {
		move(getYawTileDirection(camera->yaw + MathUtils::DEG_180));
	} else if (inputManager->isKeyPressed(Soft::Keys::D)) {
		move(getYawTileDirection(camera->yaw + MathUtils::DEG_270));
	}
}

TileDirection GameScene::getYawTileDirection(float yaw) {
	using namespace MathUtils;

	float wrappedYaw = modf(yaw, DEG_360);

	if (wrappedYaw < DEG_45 || wrappedYaw >= DEG_315) {
		return TileDirection::UP;
	} else if (wrappedYaw < DEG_135 && wrappedYaw >= DEG_45) {
		return TileDirection::LEFT;
	} else if (wrappedYaw < DEG_225 && wrappedYaw >= DEG_135) {
		return TileDirection::DOWN;
	} else if (wrappedYaw < DEG_315 && wrappedYaw >= DEG_225) {
		return TileDirection::RIGHT;
	}

	return TileDirection::UP;
}

bool GameScene::isMoving() {
	return camera->isTweening();
}

void GameScene::move(TileDirection direction) {
	if (isMoving()) {
		return;
	}

	Soft::Vec3 movementTarget = camera->position;

	switch (direction) {
		case TileDirection::UP:
			movementTarget.z += GameScene::TILE_SIZE;
			break;
		case TileDirection::DOWN:
			movementTarget.z -= GameScene::TILE_SIZE;
			break;
		case TileDirection::LEFT:
			movementTarget.x -= GameScene::TILE_SIZE;
			break;
		case TileDirection::RIGHT:
			movementTarget.x += GameScene::TILE_SIZE;
			break;
	}

	camera->tweenTo(movementTarget, MOVE_STEP_DURATION, Soft::Ease::linear);
}
