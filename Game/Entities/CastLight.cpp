#include <Entities/CastLight.h>
#include <GameUtils.h>
#include <SoftEngine.h>

/**
 * CastLight
 * ---------
 */
CastLight::CastLight(const Soft::Vec3& spawnPosition, const Soft::Vec3& direction, const Soft::Camera* camera, Soft::TextureBuffer* texture) {
	this->spawnPosition = spawnPosition;
	this->direction = direction;
	this->camera = camera;
	this->texture = texture;
}

void CastLight::initialize() {
	Soft::Light* light = new Soft::Light();

	light->setColor({ 0, 255, 0 });
	light->position = spawnPosition + direction * 100.0f;
	light->power = 1.5f;
	light->range = GameUtils::CAST_LIGHT_RANGE;

	light->onUpdate = [=](int dt) {
		light->position += direction * (float)dt * 1.5f;
	};

	Soft::Billboard* orb = new Soft::Billboard(15.0f, 15.0f);

	orb->lockTo(light);
	orb->alwaysFaceToward(camera);
	orb->setTexture(texture);
	orb->hasLighting = false;

	add(light);
	add(orb);
}
