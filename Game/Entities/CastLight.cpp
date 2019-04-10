#include <Entities/CastLight.h>
#include <GameUtils.h>
#include <SoftEngine.h>
#include <Helpers.h>

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

	light->setColor({ 0, 0, 255 });
	light->position = spawnPosition + direction * 100.0f;
	light->power = 4.0f;
	light->range = GameUtils::CAST_LIGHT_RANGE;
	light->power.tweenTo(0.0f, GameUtils::CAST_LIGHT_LIFETIME, Soft::Ease::quadInOut);

	light->onUpdate = [=](int dt) {
		light->position += direction * (float)dt * getSpeedFactor();
	};

	Soft::Billboard* orb = new Soft::Billboard(15.0f, 15.0f);

	orb->lockTo(light);
	orb->rotate({ camera->pitch, 0.0f, 0.0f });
	orb->alwaysFaceToward(camera);
	orb->setTexture(texture);
	orb->hasLighting = false;

	orb->onUpdate = [=](int dt) {
		orb->scale(0.99f);
	};

	add(light);
	add(orb);
}

float CastLight::getSpeedFactor() {
	float speedFactor = 1.0f - ((float)getAge() / GameUtils::CAST_LIGHT_COOLDOWN_TIME);

	return FAST_CLAMP(speedFactor, 0.0f, 1.0f);
}
