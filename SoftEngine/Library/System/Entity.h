#pragma once

#include <System/Objects.h>
#include <System/ParticleSystem.h>
#include <Sound/Sound.h>
#include <UI/UIObjects.h>
#include <vector>
#include <string>

namespace Soft {

class Scene;

class Entity {
public:
	int lifetime = -1;

	virtual ~Entity();

	int getAge() const;
	const std::vector<Entity*>& getQueuedEntities() const;
	const std::vector<Object*>& getQueuedObjects() const;
	const std::vector<ParticleSystem*>& getQueuedParticleSystems() const;
	const std::vector<Sound*>& getQueuedSounds() const;
	const std::vector<UIObject*>& getQueuedUIObjects() const;
	virtual void initialize() = 0;

	template<class T>
	bool isOfType() {
		return dynamic_cast<T*>(this) != nullptr;
	}

	virtual void onUpdate(int dt);
	void setActiveScene(Scene* scene);
	void update(int dt);

protected:
	void add(Entity* entity);
	void add(Object* object);
	void add(Sound* sound);
	void add(ParticleSystem* particleSystem);
	void add(UIObject* uiObject);

private:
	Scene* activeScene = nullptr;
	int age = 0;
	std::vector<Entity*> queuedEntities;
	std::vector<Object*> queuedObjects;
	std::vector<ParticleSystem*> queuedParticleSystems;
	std::vector<Sound*> queuedSounds;
	std::vector<UIObject*> queuedUIObjects;
};

} // namespace Soft
