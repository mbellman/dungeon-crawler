#pragma once

#include <MathUtils.h>
#include <SoftEngine.h>
#include <vector>

struct GridPosition {
	int layer = 0;
	int x = 0;
	int z = 0;

	bool operator ==(const GridPosition& position);
};

struct SpawnPosition : GridPosition {
	MathUtils::Direction direction = MathUtils::Direction::FORWARD;
};

struct StaticLight {
	GridPosition position;
	Soft::Color color;
	float range = 500.0f;
};

struct LayerData {
	std::vector<int> blockTypes;
};

struct LevelData {
	Soft::Area size;
	SpawnPosition spawnPosition;
	Soft::Color ambientLightColor;
	Soft::Vec3 ambientLightVector;
	float ambientLightFactor;
	int visibility;
	float brightness;
	std::vector<LayerData> layers;
	std::vector<StaticLight> staticLights;
};

class LevelLoader : Soft::Loader {
public:
	LevelLoader(const char* path);
	~LevelLoader();

	const LevelData& getLevelData();

private:
	LevelData levelData;

	void parseAmbientLightSettings();
	void parseBrightness();
	void parseLayerSize();
	void parseLayerData();
	void parseSpawnPosition();
	void parseStaticLight();
	void parseVisibility();
};
