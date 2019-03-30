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
	MathUtils::Direction direction;
};

struct StaticLight {
	GridPosition position;
	Soft::Color color;
	float range;
};

struct Actionable {
	GridPosition position;
	MathUtils::Direction direction;
};

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
	std::vector<StaticLight> staticLights;
	std::vector<Actionable> actionables;
};

class LevelLoader : Soft::Loader {
public:
	LevelLoader(const char* path);
	~LevelLoader();

	const LevelData& getLevelData();

private:
	LevelData levelData;

	MathUtils::Direction getDirection(int code);
	void parseActionable();
	void parseAmbientLightSettings();
	void parseBrightness();
	void parseLayerSize();
	void parseLayerData();
	void parseSpawnPosition();
	void parseStaticLight();
	void parseVisibility();
};
