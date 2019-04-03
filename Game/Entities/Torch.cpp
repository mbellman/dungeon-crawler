#include <Entities/Torch.h>
#include <Level/LevelLoader.h>
#include <GameUtils.h>
#include <SoftEngine.h>

/**
 * Torch
 * -----
 */
Torch::Torch(const TorchData& torchData, Soft::TextureBuffer* fireTexture) {
	this->torchData = torchData;
	this->fireTexture = fireTexture;
}

void Torch::initialize() {
	addTorchBase();
	addTorchLight();
	addFire();
	addEmbers();
}

void Torch::onUpdate(int dt) {
	int fireTextureFrame = (getAge() / 70) % fire->texture->getTotalFrames();

	fire->texture->setCurrentFrame(fireTextureFrame);
}

void Torch::addEmbers() {
	Soft::ParticleSystem* embers = new Soft::ParticleSystem(15);
	Soft::Vec3 firePosition = getFirePosition();

	embers->setParticleColor({ 255, 100, 10 });
	embers->setParticleSize(1.5f, 1.5f);

	embers->setSpawnRange(
		{ firePosition.x - 5.0f, firePosition.x + 5.0f },
		{ firePosition.y, firePosition.y },
		{ firePosition.z - 5.0f, firePosition.z + 5.0f }
	);

	embers->setParticleBehavior([=](Soft::Particle* particle, int dt) {
		particle->position.y += 0.05f * dt;

		if (
			particle->position.y - firePosition.y > 30.0f &&
			Soft::RNG::random(0.0f, 10.0f) < 1.0f
		) {
			particle->shouldReset = true;
		}
	});

	add(embers);
}

void Torch::addFire() {
	fire = new Soft::Billboard(25.0f, 25.0f);
	fire->position = getFirePosition();
	fire->rotateDeg({ 0.0f, getRotationAngle(), 0.0f });
	fire->setTexture(fireTexture);
	fire->hasLighting = false;

	add(fire);
}

void Torch::addTorchBase() {
	Soft::ObjLoader torchBaseLoader("./Assets/Models/torch-base.obj");

	Soft::Model* torchBase = new Soft::Model(torchBaseLoader);

	torchBase->rotateDeg({ 0.0f, getRotationAngle(), 0.0f });
	torchBase->setColor({ 50, 50, 50 });
	torchBase->scale(20.0f);
	torchBase->position = getTorchBasePosition();
	torchBase->fresnelFactor = 1.0f;

	add(torchBase);
}

void Torch::addTorchLight() {
	Soft::Light* torchLight = new Soft::Light();

	torchLight->setColor({ 255, 120, 30 });
	torchLight->position = getTorchLightPosition();
	torchLight->range = 400.0f;

	torchLight->onUpdate = [=](int dt) {
		torchLight->power = 1.1f + sinf(getAge() / 50.0f) * 0.075f;
	};

	add(torchLight);
}

Soft::Vec3 Torch::getFirePosition() {
	Soft::Vec3 position = GameUtils::getGridPositionVec3(torchData.position);
	Soft::Vec3 offset = getTorchDirectionVector() * (GameUtils::HALF_TILE_SIZE - 7.0f);

	position += offset;
	position.y += 38.0f;

	return position;
}

Soft::Vec3 Torch::getTorchBasePosition() {
	Soft::Vec3 position = GameUtils::getGridPositionVec3(torchData.position);
	Soft::Vec3 offset = getTorchDirectionVector() * GameUtils::HALF_TILE_SIZE;

	position += offset;
	position.y += 20.0f;

	return position;
}

Soft::Vec3 Torch::getTorchDirectionVector() {
	switch (torchData.direction) {
		case MathUtils::Direction::FORWARD:
			return { 0.0f, 0.0f, 1.0f };
		case MathUtils::Direction::BACKWARD:
			return { 0.0f, 0.0f, -1.0f };
		case MathUtils::Direction::LEFT:
			return { -1.0f, 0.0f, 0.0f };
		case MathUtils::Direction::RIGHT:
			return { 1.0f, 0.0f, 0.0f };
		default:
			return { 0.0f, 0.0f, 1.0f };
	}
}

Soft::Vec3 Torch::getTorchLightPosition() {
	Soft::Vec3 position = GameUtils::getGridPositionVec3(torchData.position);
	Soft::Vec3 offset = getTorchDirectionVector() * (GameUtils::HALF_TILE_SIZE - 25.0f);

	position += offset;
	position.y += 35.0f;

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
