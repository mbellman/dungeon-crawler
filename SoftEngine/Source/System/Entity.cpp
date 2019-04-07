#include <System/Entity.h>
#include <System/Objects.h>
#include <System/Scene.h>
#include <Sound/Sound.h>
#include <UI/UI.h>
#include <UI/UIObjects.h>
#include <Helpers.h>
#include <vector>
#include <map>
#include <string>

namespace Soft {

/**
 * Entity
 * ------
 */
Entity::~Entity() {
	queuedObjects.clear();
	queuedEntities.clear();
	queuedParticleSystems.clear();
	queuedSounds.clear();
	queuedUIObjects.clear();
}

void Entity::onUpdate(int dt) {}

void Entity::add(Entity* entity) {
	queuedEntities.push_back(entity);
}

void Entity::add(Object* object) {
	queuedObjects.push_back(object);
}

void Entity::add(ParticleSystem* particleSystem) {
	queuedParticleSystems.push_back(particleSystem);
}

void Entity::add(Sound* sound) {
	queuedSounds.push_back(sound);
}

void Entity::add(UIObject* uiObject) {
	queuedUIObjects.push_back(uiObject);
}

int Entity::getAge() const {
	return age;
}

const std::vector<Entity*>& Entity::getQueuedEntities() const {
	return queuedEntities;
}

const std::vector<Object*>& Entity::getQueuedObjects() const {
	return queuedObjects;
}

const std::vector<ParticleSystem*>& Entity::getQueuedParticleSystems() const {
	return queuedParticleSystems;
}

const std::vector<Sound*>& Entity::getQueuedSounds() const {
	return queuedSounds;
}

const std::vector<UIObject*>& Entity::getQueuedUIObjects() const {
	return queuedUIObjects;
}

void Entity::update(int dt) {
	if (lifetime > 0) {
		lifetime = FAST_MAX(lifetime - dt, 0);
	}

	age += dt;

	onUpdate(dt);
}

} // namespace Soft
