#include <Level/LevelLoader.h>
#include <MathUtils.h>
#include <string>

static std::string LAYER_SIZE = "LS";
static std::string SPAWN_POSITION= "SP";
static std::string AMBIENT_LIGHT = "AL";
static std::string VISIBILITY = "V";
static std::string BRIGHTNESS = "B";
static std::string STATIC_LIGHT = "SL";
static std::string ACTIONABLE = "A";
static std::string LAYER = "L";

/**
 * GridPosition
 * ------------
 */
bool GridPosition::operator ==(const GridPosition& position) {
	return layer == position.layer && x == position.x && z == position.z;
}

/**
 * LevelLoader
 * -----------
 */
LevelLoader::LevelLoader(const char* path) {
	load(path);

	while(isLoading) {
		setChunkDelimiter(" ");

		std::string label = readNextChunk();

		if (label == LAYER_SIZE) {
			parseLayerSize();
		} else if (label == SPAWN_POSITION) {
			parseSpawnPosition();
		} else if (label == AMBIENT_LIGHT) {
			parseAmbientLightSettings();
		} else if (label == VISIBILITY) {
			parseVisibility();
		} else if (label == BRIGHTNESS) {
			parseBrightness();
		} else if (label == STATIC_LIGHT) {
			parseStaticLight();
		} else if (label == ACTIONABLE) {
			parseActionable();
		} else if (label == LAYER) {
			parseLayerData();
		}

		nextLine();
	}
}

LevelLoader::~LevelLoader() {
	levelData.layers.clear();
	levelData.staticLights.clear();
	levelData.actionables.clear();
}

MathUtils::Direction LevelLoader::getDirection(int code) {
	switch (code) {
		case 0:
			return MathUtils::Direction::FORWARD;
		case 1:
			return MathUtils::Direction::LEFT;
		case 2:
			return MathUtils::Direction::BACKWARD;
		case 3:
			return MathUtils::Direction::RIGHT;
		default:
			return MathUtils::Direction::FORWARD;
	}
}

const LevelData& LevelLoader::getLevelData() {
	return levelData;
}

void LevelLoader::parseActionable() {
	setChunkDelimiter(",");

	Actionable actionable;

	actionable.position.layer = std::stoi(readNextChunk());
	actionable.position.x = std::stoi(readNextChunk());
	actionable.position.z = std::stoi(readNextChunk());
	actionable.direction = getDirection(std::stoi(readNextChunk()));

	levelData.actionables.push_back(actionable);
}

void LevelLoader::parseAmbientLightSettings() {
	setChunkDelimiter(",");

	levelData.ambientLightColor.R = std::stoi(readNextChunk());
	levelData.ambientLightColor.G = std::stoi(readNextChunk());
	levelData.ambientLightColor.B = std::stoi(readNextChunk());
	levelData.ambientLightVector.x = std::stof(readNextChunk());
	levelData.ambientLightVector.y = std::stof(readNextChunk());
	levelData.ambientLightVector.z = std::stof(readNextChunk());
	levelData.ambientLightFactor = std::stof(readNextChunk());
}

void LevelLoader::parseBrightness() {
	levelData.brightness = std::stof(readNextChunk());
}

void LevelLoader::parseLayerSize() {
	setChunkDelimiter(",");

	levelData.size.width = std::stoi(readNextChunk());
	levelData.size.height = std::stoi(readNextChunk());
}

void LevelLoader::parseLayerData() {
	setChunkDelimiter(",");

	LayerData layer;

	for (int z = 0; z < levelData.size.height; z++) {
		for (int x = 0; x < levelData.size.width; x++) {
			layer.blockTypes.push_back(std::stoi(readNextChunk()));
		}
	}

	levelData.layers.push_back(layer);
}

void LevelLoader::parseSpawnPosition() {
	setChunkDelimiter(",");

	levelData.spawnPosition.layer = std::stoi(readNextChunk());
	levelData.spawnPosition.x = std::stoi(readNextChunk());
	levelData.spawnPosition.z = std::stoi(readNextChunk());
	levelData.spawnPosition.direction = getDirection(std::stoi(readNextChunk()));
}

void LevelLoader::parseStaticLight() {
	setChunkDelimiter(",");

	StaticLight staticLight;

	staticLight.position.layer = std::stoi(readNextChunk());
	staticLight.position.x = std::stoi(readNextChunk());
	staticLight.position.z = std::stoi(readNextChunk());
	staticLight.color.R = std::stoi(readNextChunk());
	staticLight.color.G = std::stoi(readNextChunk());
	staticLight.color.B = std::stoi(readNextChunk());
	staticLight.range = std::stof(readNextChunk());

	levelData.staticLights.push_back(staticLight);
}

void LevelLoader::parseVisibility() {
	levelData.visibility = std::stoi(readNextChunk());
}
