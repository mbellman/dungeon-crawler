#include <System/Scene.h>
#include <System/Entity.h>
#include <System/Objects.h>
#include <System/InputManager.h>
#include <System/Camera.h>
#include <System/Controller.h>
#include <string>
#include <vector>
#include <algorithm>
#include <functional>
#include <cmath>
#include <UI/UI.h>
#include <UI/Alert.h>
#include <Graphics/TextureBuffer.h>
#include <Sound/Sound.h>
#include <Constants.h>

namespace Soft {

/**
 * Scene
 * -----
 */
Scene::Scene() {
	boot();
}

Scene::~Scene() {
	unload();
}

void Scene::add(Entity* entity) {
	entities.push_back(entity);

	entity->setActiveScene(this);
	entity->initialize();
}

void Scene::add(Object* object) {
	object->syncLODs();
	object->recomputeSurfaceNormals();

	objects.push_back(object);

	if (object->isOfType<Light>()) {
		lights.push_back((Light*)object);
	}
}

void Scene::add(ParticleSystem* particleSystem) {
	particleSystems.push_back(particleSystem);

	for (auto* particle : particleSystem->getParticles()) {
		objects.push_back(particle);
	}
}

void Scene::add(Sound* sound) {
	sounds.push_back(sound);
}

void Scene::add(const Sector& sector) {
	sectors.push_back(sector);
}

void Scene::add(const char* key, Entity* entity) {
	entityMap.emplace(key, entity);
	add(entity);
}

void Scene::add(const char* key, Object* object) {
	objectMap.emplace(key, object);
	add(object);
}

void Scene::add(const char* key, ParticleSystem* particleSystem) {
	particleSystemMap.emplace(key, particleSystem);
	add(particleSystem);
}

void Scene::add(const char* key, ObjLoader* objLoader) {
	objLoaderMap.emplace(key, objLoader);
}

void Scene::add(const char* key, TextureBuffer* textureBuffer) {
	textureBufferMap.emplace(key, textureBuffer);
}

void Scene::add(const char* key, Sound* sound) {
	soundMap.emplace(key, sound);
	add(sound);
}

void Scene::boot() {
	runningTime = 0;
	isPaused = false;
	hasInitialized = false;

	inputManager = new InputManager();
	camera = new Camera();

	inputManager->onMouseMotion([=](int dx, int dy) {
		handleMouseMotion(dx, dy);
	});
}

void Scene::emptyDisposalQueues() {
	for (auto* object : objectDisposalQueue) {
		delete object;
	}

	for (auto* particleSystem : particleSystemDisposalQueue) {
		delete particleSystem;
	}

	objectDisposalQueue.clear();
	particleSystemDisposalQueue.clear();
}

const Camera& Scene::getCamera() const {
	return *camera;
}

const std::vector<Light*>& Scene::getLights() {
	return lights;
}

Object* Scene::getObject(const char* key) {
	return retrieveMappedItem(objectMap, key);
}

const std::vector<Object*>& Scene::getObjects() {
	return objects;
}

ObjLoader* Scene::getObjLoader(const char* key) {
	return retrieveMappedItem(objLoaderMap, key);
}

ParticleSystem* Scene::getParticleSystem(const char* key) {
	return retrieveMappedItem(particleSystemMap, key);
}

int Scene::getRunningTime() {
	return runningTime;
}

Sound* Scene::getSound(const char* key) {
	return retrieveMappedItem(soundMap, key);
}

const std::vector<Sound*>& Scene::getSounds() {
	return sounds;
}

TextureBuffer* Scene::getTexture(const char* key) {
	return retrieveMappedItem(textureBufferMap, key);
}

void Scene::handleControl(int dt) {
	if (settings.controlMode & ControlMode::WASD) {
		handleWASDControl(dt);
	}
}

void Scene::handleMouseMotion(int dx, int dy) {
	if (isPaused || ~settings.controlMode & ControlMode::MOUSE) {
		return;
	}

	float deltaFactor = (1.0f / 500.0f) * camera->mouseSensitivity;
	float yawDelta = (float)-dx * deltaFactor;
	float pitchDelta = (float)-dy * deltaFactor;

	camera->yaw += yawDelta;
	camera->pitch = std::clamp(camera->pitch + pitchDelta, -MAX_CAMERA_PITCH, MAX_CAMERA_PITCH);
}

void Scene::handleWASDControl(int dt) {
	Vec3 velocity;

	if (inputManager->isKeyPressed(Keys::W)) {
		velocity.z = 1;
	} else if (inputManager->isKeyPressed(Keys::S)) {
		velocity.z = -1;
	}

	if (inputManager->isKeyPressed(Keys::A)) {
		velocity.x = -1;
	} else if (inputManager->isKeyPressed(Keys::D)) {
		velocity.x = 1;
	}

	velocity.normalize();
	velocity *= camera->movementSpeed * (dt / 16.0f);

	if (inputManager->isKeyPressed(Keys::SHIFT)) {
		velocity *= 4.0f;
	}

	float sy = sinf(camera->yaw);
	float cy = cosf(camera->yaw);

	camera->position.x += cy * velocity.x + sy * -velocity.z;
	camera->position.z += cy * velocity.z + sy * velocity.x;
}

bool Scene::isInCurrentOccupiedSector(int sectorId) {
	if (sectorId == GLOBAL_SECTOR_ID) {
		return true;
	}

	for (int occupiedSectorId : currentOccupiedSectors) {
		if (occupiedSectorId == sectorId) {
			return true;
		}
	}

	return false;
}

void Scene::onStart() {}
void Scene::onUpdate(int dt) {}

void Scene::provideController(Controller* controller) {
	this->controller = controller;
}

void Scene::provideUI(UI* ui) {
	this->ui = ui;
}

/**
 * Removes an item by key, agnostic as to the item type.
 * We invoke all routines for safely freeing mapped items,
 * deferring to their mechanisms for verifying the existence
 * of and freeing resources.
 */
void Scene::remove(const char* key) {
	safelyFreeMappedObject(key);
	safelyFreeMappedSound(key);
	safelyFreeMappedParticleSystem(key);
	safelyFreeMappedEntity(key);

	safelyFreeMappedItem(objLoaderMap, key);
	safelyFreeMappedItem(textureBufferMap, key);
	safelyFreeMappedItem(particleSystemMap, key);
}

void Scene::removeEntity(Entity* entity) {
	for (auto* entity : entity->getQueuedEntities()) {
		removeEntity(entity);
	}

	for (auto* object : entity->getQueuedObjects()) {
		removeObject(object);
	}

	for (auto* particleSystem : entity->getQueuedParticleSystems()) {
		removeParticleSystem(particleSystem);
	}

	for (auto* sound : entity->getQueuedSounds()) {
		removeSound(sound);
	}

	for (auto* uiObject : entity->getQueuedUIObjects()) {
		ui->remove(uiObject);
	}

	entities.erase(std::remove(entities.begin(), entities.end(), entity), entities.end());

	delete entity;
}

void Scene::removeExpiredItems() {
	int idx = 0;

	while (idx < objects.size()) {
		Object* object = objects.at(idx);

		if (object->lifetime == 0) {
			removeObject(object);
		} else {
			idx++;
		}
	}

	idx = 0;

	while (idx < entities.size()) {
		Entity* entity = entities.at(idx);

		if (entity->lifetime == 0) {
			removeEntity(entity);
		} else {
			idx++;
		}
	}
}

/**
 * Removes an Object by reference from the Object pointer
 * list, and Light pointer list if applicable, before placing
 * it in the disposal queue for deferred deletion.
 */
void Scene::removeObject(Object* object) {
	objects.erase(std::remove(objects.begin(), objects.end(), object), objects.end());

	if (object->isOfType<Light>()) {
		lights.erase(std::remove(lights.begin(), lights.end(), object), lights.end());
	}

	objectDisposalQueue.push_back(object);
}

/**
 * Removes a ParticleSystem by erasing all of its Particle Objects
 * from the Object pointer list, and defers the ParticleSystem
 * itself for deletion, which ultimately frees the allocated Particles.
 * Since all of a ParticleSystem's Particles are stored contiguously
 * in the Object pointer list, we can erase all list elements from
 * [N = first particle index, N + particle system size).
 */
void Scene::removeParticleSystem(ParticleSystem* particleSystem) {
	particleSystems.erase(std::remove(particleSystems.begin(), particleSystems.end(), particleSystem), particleSystems.end());

	const std::vector<Particle*>& particles = particleSystem->getParticles();
	Particle* firstParticle = particles.at(0);
	int idx = -1;

	while (++idx < objects.size()) {
		if (objects.at(idx) == firstParticle) {
			objects.erase(objects.begin() + idx, objects.begin() + idx + particles.size());

			break;
		}
	}

	particleSystemDisposalQueue.push_back(particleSystem);
}

/**
 * Removes a Sound by reference from the Sound pointer list.
 */
void Scene::removeSound(Sound* sound) {
	sounds.erase(std::remove(sounds.begin(), sounds.end(), sound), sounds.end());

	delete sound;
}

/**
 * Flags the Scene to reset at the end of its update cycle.
 * A reset returns the Scene to its instantiation-time state,
 * prior to load() or onStart() calls. Reload/restart will
 * occur only once the Scene becomes the active one again,
 * either at the beginning of the next Controller update cycle
 * if no Scene change occurs, or if the changed Scene(s) are
 * exited.
 */
void Scene::reset() {
	shouldReset = true;
}

void Scene::resume() {
	for (auto* sound : sounds) {
		sound->resume();
	}
}

void Scene::safelyFreeMappedEntity(const char* key) {
	const auto& entry = entityMap.find(key);

	if (entry != entityMap.end()) {
		Entity* entity = entry->second;

		entityMap.erase(key);
		removeEntity(entity);
	}
}

/**
 * Removes and destroys a mapped Object if the provided key matches
 * an entry. The Object must be deleted in the following fashion:
 *
 *  A) From the Object map
 *  B) From the Object pointer list
 *  C) If it is a Light, from the Lights pointer list
 *  D) Added to the disposal queue for deferred deletion
 */
void Scene::safelyFreeMappedObject(const char* key) {
	const auto& entry = objectMap.find(key);

	if (entry != objectMap.end()) {
		Object* object = entry->second;

		objectMap.erase(key);
		removeObject(object);
	}
}

/**
 * Removes a mapped ParticleSystem if the provided key matches an entry.
 */
void Scene::safelyFreeMappedParticleSystem(const char* key) {
	const auto& entry = particleSystemMap.find(key);

	if (entry != particleSystemMap.end()) {
		ParticleSystem* particleSystem = particleSystemMap.at(key);

		particleSystemMap.erase(key);
		removeParticleSystem(particleSystem);
	}
}

void Scene::safelyFreeMappedSound(const char* key) {
	const auto& entry = soundMap.find(key);

	if (entry != soundMap.end()) {
		Sound* sound = entry->second;

		soundMap.erase(key);
		removeSound(sound);
	}
}

void Scene::suspend() {
	inputManager->resetKeyState();

	for (auto* sound : sounds) {
		sound->pause();
	}
}

void Scene::togglePause() {
	isPaused = !isPaused;
}

void Scene::update(int dt) {
	if (isPaused || !hasInitialized) {
		return;
	}

	runningTime += dt;

	handleControl(dt);
	camera->update(dt);
	updateCurrentOccupiedSectors();

	for (auto* entity : entities) {
		entity->update(dt);
	}

	for (auto* object : objects) {
		object->update(dt);
	}

	for (auto* particleSystem : particleSystems) {
		particleSystem->update(dt);
	}

	emptyDisposalQueues();
	onUpdate(dt);
	removeExpiredItems();

	for (auto* object : objects) {
		object->syncLODs();
	}

	if (shouldReset) {
		unload();
		boot();

		shouldReset = false;
	}
}

void Scene::unload() {
	for (auto* entity : entities) {
		delete entity;
	}

	for (auto* object : objects) {
		if (!object->isOfType<Particle>()) {
			// Only delete Objects other than Particles, which
			// are managed by their source ParticleSystems
			delete object;
		}
	}

	for (auto* particleSystem : particleSystems) {
		delete particleSystem;
	}

	for (auto* sound : sounds) {
		delete sound;
	}

	for (auto& [key, objLoader] : objLoaderMap) {
		delete objLoader;
	}

	for (auto& [key, textureBuffer] : textureBufferMap) {
		delete textureBuffer;
	}

	delete inputManager;
	delete camera;

	if (ui != NULL) {
		// If a reset (but not reloaded/restarted) Scene is further back
		// in the Scene stack, its 'ui' field will be deleted already.
		// Quitting the application in this state would cause a double
		// free if we didn't guard the deletion with a NULL check.
		delete ui;

		ui = NULL;
	}

	entities.clear();
	objects.clear();
	particleSystems.clear();
	lights.clear();
	sounds.clear();
	sectors.clear();

	entityMap.clear();
	objectMap.clear();
	objLoaderMap.clear();
	textureBufferMap.clear();
	particleSystemMap.clear();

	emptyDisposalQueues();
}

void Scene::updateCurrentOccupiedSectors() {
	currentOccupiedSectors.clear();

	for (const auto& sector : sectors) {
		if (sector.bounds.hasPointInside(camera->position)) {
			currentOccupiedSectors.push_back(sector.id);
		}
	}
}

} // namespace Soft
