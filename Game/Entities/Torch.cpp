#include <Entities/Torch.h>
#include <Level/LevelLoader.h>
#include <GameUtils.h>
#include <SoftEngine.h>

/**
 * Torch
 * -----
 */
Torch::Torch(const TorchData& torchData) {
	this->torchData = torchData;
}

void Torch::initialize() {
	Soft::ObjLoader torchBaseLoader("./Assets/Models/torch-base.obj");

	Soft::Model* torchBase = new Soft::Model(torchBaseLoader);

	torchBase->rotateDeg({ 0.0f, getRotationAngle(), 0.0f });
	torchBase->setColor({ 50, 50, 50 });
	torchBase->scale(20.0f);
	torchBase->position = getTorchBasePosition();
	torchBase->fresnelFactor = 1.0f;

	Soft::Light* torchLight = new Soft::Light();

	torchLight->setColor({ 255, 120, 30 });
	torchLight->position = getTorchLightPosition();
	torchLight->range = 450.0f;

	torchLight->onUpdate = [=](int dt) {
		torchLight->power = 1.75f + sinf(getAge() / 50.0f) * 0.1f;
	};

	Soft::DirectionalLight* light = new Soft::DirectionalLight();

	light->setDirection({ 0.0f, 1.0f, 0.0f });
	light->setColor({ 255, 0, 0 });
	light->position = torchLight->position;
	light->power = 5.0f;

	Soft::Billboard* fire = new Soft::Billboard(10.0f, 10.0f);
	fire->position = torchLight->position;
	fire->setColor({ 255, 0, 0 });
	fire->rotateDeg({ 0.0f, getRotationAngle(), 0.0f });
	fire->hasLighting = false;

	add(torchBase);
	add(torchLight);
	add(light);
	add(fire);
}

void Torch::onUpdate(int dt) {

}

Soft::Vec3 Torch::getTorchBasePosition() {
	Soft::Vec3 position = GameUtils::getGridPositionVec3(torchData.position);

	position.y += 20.0f;

	switch (torchData.direction) {
		case MathUtils::Direction::FORWARD:
			position.z += GameUtils::HALF_TILE_SIZE;
			break;
		case MathUtils::Direction::BACKWARD:
			position.z -= GameUtils::HALF_TILE_SIZE;
			break;
		case MathUtils::Direction::LEFT:
			position.x -= GameUtils::HALF_TILE_SIZE;
			break;
		case MathUtils::Direction::RIGHT:
			position.x += GameUtils::HALF_TILE_SIZE;
			break;
	}

	return position;
}

Soft::Vec3 Torch::getTorchLightPosition() {
	Soft::Vec3 position = getTorchBasePosition();
	float offset = 40.0f;

	position.y += 20.0f;

	switch (torchData.direction) {
		case MathUtils::Direction::FORWARD:
			position.z -= offset;
			break;
		case MathUtils::Direction::BACKWARD:
			position.z += offset;
			break;
		case MathUtils::Direction::LEFT:
			position.x += offset;
			break;
		case MathUtils::Direction::RIGHT:
			position.x -= offset;
			break;
	}

	return position;
}

float Torch::getRotationAngle() {
	switch (torchData.direction) {
		case MathUtils::Direction::LEFT:
			return 90.0f;
		case MathUtils::Direction::FORWARD:
			return 180.0f;
		case MathUtils::Direction::RIGHT:
			return 270.0f;
		default:
			return 0.0f;
	}
}
