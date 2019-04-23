#include <Entities/Desecration.h>
#include <Level/LevelLoader.h>
#include <GameUtils.h>

/**
 * Desecration
 * -----------
 */
Desecration::Desecration(const DesecrationData& desecrationData): Interactible(desecrationData) {}

void Desecration::initialize() {
	auto spawnRange = getSpawnRange();
	Soft::ParticleSystem* particles = new Soft::ParticleSystem(50);

	particles->setParticleColor({ 255, 0, 0 });
	particles->setParticleSize(2, 2);

	particles->setSpawnRange(
		{ spawnRange.start.x, spawnRange.end.x },
		{ spawnRange.start.y, spawnRange.end.y },
		{ spawnRange.start.z, spawnRange.end.z }
	);

	particles->setParticleBehavior([=](Soft::Particle* particle, int dt) {
		particle->position.y += 0.05f * dt;

		if (particle->position.y - spawnRange.start.y > GameUtils::TILE_SIZE) {
			particle->shouldReset = true;
		}
	});

	Soft::Light* light = new Soft::Light();

	light->position = GameUtils::getGridPositionVec3(data.position);
	light->setColor({ 255, 0, 0 });
	light->isStatic = true;
	light->power = 0.75f;

	add(particles);
	add(light);
}

Soft::Range<Soft::Vec3> Desecration::getSpawnRange() {
	Soft::Range<Soft::Vec3> spawnRange;
	Soft::Vec3 position = GameUtils::getGridPositionVec3(data.position);

	spawnRange.start = {
		position.x - GameUtils::HALF_TILE_SIZE,
		position.y - GameUtils::HALF_TILE_SIZE,
		position.z - GameUtils::HALF_TILE_SIZE
	};

	spawnRange.end = {
		position.x + GameUtils::HALF_TILE_SIZE,
		position.y + GameUtils::HALF_TILE_SIZE,
		position.z + GameUtils::HALF_TILE_SIZE
	};

	return spawnRange;
}
