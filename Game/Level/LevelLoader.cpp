#include <Level/LevelLoader.h>
#include <MathUtils.h>
#include <string>

static std::string LAYER_SIZE = "LS";
static std::string SPAWN_POSITION= "SP";
static std::string VISIBILITY = "V";
static std::string BRIGHTNESS = "B";
static std::string STATIC_LIGHT = "SL";
static std::string LAYER = "L";

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
		} else if (label == VISIBILITY) {
			parseVisibility();
		} else if (label == BRIGHTNESS) {
			parseBrightness();
		} else if (label == STATIC_LIGHT) {
			parseStaticLight();
		} else if (label == LAYER) {
			parseLayerData();
		}

		nextLine();
	}
}

LevelLoader::~LevelLoader() {

}

const LevelData& LevelLoader::getLevelData() {
	return levelData;
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

	SpawnPosition& position = levelData.spawnPosition;

	position.layer = std::stoi(readNextChunk());
	position.x = std::stoi(readNextChunk());
	position.z = std::stoi(readNextChunk());

	switch (std::stoi(readNextChunk())) {
		case 0:
			position.direction = MathUtils::Direction::FORWARD;
			break;
		case 1:
			position.direction = MathUtils::Direction::LEFT;
			break;
		case 2:
			position.direction = MathUtils::Direction::BACKWARD;
			break;
		case 3:
			position.direction = MathUtils::Direction::RIGHT;
			break;
	}
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
