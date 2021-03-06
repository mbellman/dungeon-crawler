#pragma once

#include <map>
#include <vector>
#include <climits>
#include <functional>
#include <Sound/Sound.h>
#include <System/Entity.h>
#include <System/Objects.h>
#include <System/Geometry.h>
#include <System/ParticleSystem.h>
#include <System/Camera.h>
#include <UI/UI.h>
#include <System/InputManager.h>
#include <Graphics/TextureBuffer.h>
#include <System/Controller.h>
#include <SDL.h>

namespace Soft {

/**
 * ControlMode
 * -----------
 */
enum ControlMode {
	WASD = 1 << 0,
	MOUSE = 1 << 1
};

/**
 * Settings
 * --------
 */
struct Settings {
	Color backgroundColor = { 0, 0, 0 };
	Color ambientLightColor = { 0, 0, 0 };
	Vec3 ambientLightVector = { 0, -1, 0 };
	float ambientLightFactor = 1.0f;
	bool hasStaticAmbientLight = false;
	float brightness = 1.0f;
	int visibility = INT_MAX;
	int controlMode = ControlMode::WASD | ControlMode::MOUSE;
};

/**
 * Scene
 * -----
 */
class Scene {
public:
	friend class Entity;

	InputManager* inputManager = NULL;
	UI* ui = NULL;
	Settings settings;
	bool hasInitialized = false;

	Scene();
	~Scene();

	const Camera& getCamera() const;
	const std::vector<Light*>& getLights();
	const std::vector<Object*>& getObjects();
	const std::vector<Sound*>& getSounds();
	bool isInCurrentOccupiedSector(int sectorId);
	virtual void load() = 0;
	virtual void onStart();
	virtual void onUpdate(int dt);
	void provideController(Controller* controller);
	void provideUI(UI* ui);
	void resume();
	void suspend();
	void togglePause();
	void update(int dt);

protected:
	Controller* controller = NULL;
	Camera* camera = NULL;

	void add(Entity* entity);
	void add(Object* object);
	void add(ParticleSystem* particleSystem);
	void add(Sound* sound);
	void add(const Sector& sector);
	void add(const char* key, Entity* entity);
	void add(const char* key, Object* object);
	void add(const char* key, ParticleSystem* particleSystem);
	void add(const char* key, ObjLoader* objLoader);
	void add(const char* key, TextureBuffer* textureBuffer);
	void add(const char* key, Sound* sound);

	template<class T>
	T* getEntity(const char* key) {
		return ((T*)retrieveMappedItem(entityMap, key));
	}

	template<class T>
	T* getObject(const char* key) {
		return ((T*)getObject(key));
	}

	Object* getObject(const char* key);
	ObjLoader* getObjLoader(const char* key);
	ParticleSystem* getParticleSystem(const char* key);
	int getRunningTime();
	Sound* getSound(const char* key);
	TextureBuffer* getTexture(const char* key);
	void remove(const char* key);
	void reset();

private:
	std::vector<Entity*> entities;
	std::vector<Object*> objects;
	std::vector<ParticleSystem*> particleSystems;
	std::vector<Light*> lights;
	std::vector<Sound*> sounds;
	std::vector<Sector> sectors;
	std::map<const char*, Entity*> entityMap;
	std::map<const char*, Object*> objectMap;
	std::map<const char*, ObjLoader*> objLoaderMap;
	std::map<const char*, TextureBuffer*> textureBufferMap;
	std::map<const char*, ParticleSystem*> particleSystemMap;
	std::map<const char*, Sound*> soundMap;

	std::vector<Object*> objectDisposalQueue;
	std::vector<ParticleSystem*> particleSystemDisposalQueue;

	std::vector<int> currentOccupiedSectors;
	int runningTime = 0;
	bool isPaused = false;
	bool shouldReset = false;

	void boot();
	void emptyDisposalQueues();
	void handleControl(int dt);
	void handleMouseMotion(int dx, int dy);
	void handleWASDControl(int dt);
	void removeEntity(Entity* entity);
	void removeExpiredItems();
	void removeObject(Object* object);
	void removeParticleSystem(ParticleSystem* particleSystem);
	void removeSound(Sound* sound);

	template<class T>
	T* retrieveMappedItem(std::map<const char*, T*> map, const char* key) {
		const auto& entry = map.find(key);

		if (entry != map.end()) {
			return entry->second;
		}

		return nullptr;
	}

	void safelyFreeMappedEntity(const char* key);

	template<class T>
	void safelyFreeMappedItem(std::map<const char*, T*> map, const char* key) {
		const auto& entry = map.find(key);

		if (entry != map.end()) {
			map.erase(key);

			delete entry->second;

			return;
		}
	}

	void safelyFreeMappedObject(const char* key);
	void safelyFreeMappedParticleSystem(const char* key);
	void safelyFreeMappedSound(const char* key);
	void unload();
	void updateCurrentOccupiedSectors();
};

} // namespace Soft
