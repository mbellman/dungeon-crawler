#include <Scenes/GameScene.h>
#include <SoftEngine.h>
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

	inputManager->onKeyDown([=](const SDL_Keycode& code) {
		onKeyDown(code);
	});

	settings.ambientLightFactor = 0.0f;
	settings.brightness = 0.2f;
	settings.controlMode = Soft::ControlMode::MOUSE;
}

void GameScene::onUpdate(int dt) {

}

float GameScene::getWrappedYaw(float yaw) {
	return yaw - DEG_360 * floorf(yaw / DEG_360);
}

TileDirection GameScene::getYawTileDirection(float yaw) {
	float wrappedYaw = getWrappedYaw(yaw);

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

void GameScene::onKeyDown(const SDL_Keycode& code) {
	switch (code) {
		case SDLK_w:
			move(getYawTileDirection(camera->yaw));
			break;
		case SDLK_a:
			move(getYawTileDirection(camera->yaw + DEG_90));
			break;
		case SDLK_s:
			move(getYawTileDirection(camera->yaw + DEG_180));
			break;
		case SDLK_d:
			move(getYawTileDirection(camera->yaw + DEG_270));
			break;
	}
}
