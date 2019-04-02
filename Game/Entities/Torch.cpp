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
	torchLight->range = 400.0f;

	torchLight->onUpdate = [=](int dt) {
		torchLight->power = 0.95f + sinf(getAge() / 50.0f) * 0.075f;
	};

	Soft::Billboard* fire = new Soft::Billboard(10.0f, 10.0f);
	fire->position = torchLight->position;
	fire->setColor({ 255, 0, 0 });
	fire->rotateDeg({ 0.0f, getRotationAngle(), 0.0f });
	fire->hasLighting = false;

	add(torchBase);
	add(torchLight);
	add(fire);

	addEmbers();
}

void Torch::onUpdate(int dt) {

}

void Torch::addEmbers() {
	Soft::ParticleSystem* embers = new Soft::ParticleSystem(15);
	Soft::Vec3 lightPosition = getTorchLightPosition();

	embers->setParticleColor({ 255, 150, 10 });
	embers->setParticleSize(2.0f, 2.0f);

	embers->setSpawnRange(
		{ lightPosition.x - 8.0f, lightPosition.x + 8.0f },
		{ lightPosition.y, lightPosition.y },
		{ lightPosition.z - 8.0f, lightPosition.z + 8.0f }
	);

	embers->setParticleBehavior([=](Soft::Particle* particle, int dt) {
		particle->position.y += 0.1f * dt;

		if (
			particle->position.y - lightPosition.y > 30.0f &&
			Soft::RNG::random(0.0f, 10.0f) < 1.0f
		) {
			particle->shouldReset = true;
		}
	});

	add(embers);
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
	float offset = 20.0f;

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
