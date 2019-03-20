#include <System/Entity.h>
#include <System/Objects.h>
#include <Sound/Sound.h>
#include <UI/UI.h>
#include <UI/UIObjects.h>
#include <Helpers.h>
#include <vector>
#include <map>

namespace Soft {

/**
 * Entity
 * ------
 */
Entity::~Entity() {
	queuedObjects.clear();
	queuedSounds.clear();
	queuedUIObjectMap.clear();
}

void Entity::onUpdate(int dt) {}

void Entity::add(Object* object) {
	queuedObjects.push_back(object);
}

void Entity::add(Sound* sound) {
	queuedSounds.push_back(sound);
}

void Entity::add(const char* key, UIObject* uiObject) {
	queuedUIObjectMap.emplace(key, uiObject);
}

const std::vector<Object*>& Entity::getQueuedObjects() const {
	return queuedObjects;
}

const std::vector<Sound*>& Entity::getQueuedSounds() const {
	return queuedSounds;
}

const std::map<const char*, UIObject*>& Entity::getQueuedUIObjectMap() const {
	return queuedUIObjectMap;
}

void Entity::update(int dt) {
	if (lifetime > 0) {
		lifetime = FAST_MAX(lifetime - dt, 0);
	}

	onUpdate(dt);
}

} // namespace Soft