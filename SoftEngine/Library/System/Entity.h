#pragma once

#include <System/Objects.h>
#include <System/ParticleSystem.h>
#include <Sound/Sound.h>
#include <UI/UIObjects.h>
#include <vector>
#include <map>
#include <string>

namespace Soft {

class Entity {
public:
	int lifetime = -1;

	~Entity();

	int getAge() const;
	const std::vector<Object*>& getQueuedObjects() const;
	const std::vector<ParticleSystem*>& getQueuedParticleSystems() const;
	const std::vector<Sound*>& getQueuedSounds() const;
	const std::map<std::string, UIObject*>& getQueuedUIObjectMap() const;
	virtual void initialize() = 0;
	virtual void onUpdate(int dt);
	void update(int dt);

protected:
	void add(Object* object);
	void add(Sound* sound);
	void add(ParticleSystem* particleSystem);
	void add(std::string, UIObject* uiObject);

private:
	int age = 0;
	std::vector<Object*> queuedObjects;
	std::vector<ParticleSystem*> queuedParticleSystems;
	std::vector<Sound*> queuedSounds;
	std::map<std::string, UIObject*> queuedUIObjectMap;
};

} // namespace Soft
