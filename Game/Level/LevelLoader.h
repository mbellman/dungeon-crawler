#pragma once

#include <MathUtils.h>
#include <SoftEngine.h>
#include <vector>

struct GridPosition {
	int layer = 0;
	int x = 0;
	int z = 0;

	bool operator ==(const GridPosition& position) const;
};

struct SpawnPosition : GridPosition {
	MathUtils::Direction direction;
};

struct EntityData {
	GridPosition position;
};

struct TorchData : EntityData {
	MathUtils::Direction direction;
};

struct ChestData : EntityData {
	MathUtils::Direction direction;
	int itemType;
};

struct DoorData : EntityData {
	GridPosition position;
	MathUtils::Axis axis;
};

struct DesecrationData : EntityData {};

struct LayerData {
	std::vector<int> blockTypes;
};

struct LevelData {
	Soft::Area size;
	SpawnPosition spawnPosition;
	Soft::Color ambientLightColor;
	Soft::Vec3 ambientLightVector;
	float ambientLightFactor = 1.0f;
	int visibility = 10000;
	float brightness = 1.0f;
	std::vector<LayerData> layers;
	std::vector<TorchData> torches;
	std::vector<ChestData> chests;
	std::vector<DoorData> doors;
	std::vector<DesecrationData> desecrations;
};

class LevelLoader : Soft::Loader {
public:
	LevelLoader(const char* path);
	~LevelLoader();

	const LevelData& getLevelData();

private:
	LevelData levelData;

	MathUtils::Axis getAxis(int code);
	MathUtils::Direction getDirection(int code);
	void parseAmbientLightSettings();
	void parseBrightness();
	void parseChestData();
	void parseDesecrationData();
	void parseDoorData();
	void parseLayerSize();
	void parseLayerData();
	void parseSpawnPosition();
	void parseTorchData();
	void parseVisibility();
};
