#include <Entities/CastLight.h>
#include <GameUtils.h>
#include <SoftEngine.h>

/**
 * CastLight
 * ---------
 */
CastLight::CastLight(const Soft::Vec3& spawnPosition, const Soft::Vec3& direction) {
	this->spawnPosition = spawnPosition;
	this->direction = direction;
}

void CastLight::initialize() {
	Soft::Light* light = new Soft::Light();

	light->setColor({ 0, 255, 0 });
	light->position = spawnPosition + direction * 100.0f;
	light->range = GameUtils::CAST_LIGHT_RANGE;

	light->onUpdate = [=](int dt) {
		light->position += direction * (float)dt * 1.5f;
	};

	Soft::Cube* lightCube = new Soft::Cube(3.0f);

	lightCube->setColor({ 0, 255, 0 });
	lightCube->hasLighting = false;
	lightCube->lockTo(light);

	add(light);
	add(lightCube);
}
