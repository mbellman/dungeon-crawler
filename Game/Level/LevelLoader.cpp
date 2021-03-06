#include <Level/LevelLoader.h>
#include <MathUtils.h>
#include <string>

static std::string LAYER_SIZE = "LS";
static std::string SPAWN_POSITION= "SP";
static std::string AMBIENT_LIGHT = "AL";
static std::string VISIBILITY = "V";
static std::string BRIGHTNESS = "B";
static std::string TORCH = "T";
static std::string CHEST = "CH";
static std::string DOOR = "D";
static std::string DESECRATION = "DE";
static std::string LAYER = "L";

/**
 * GridPosition
 * ------------
 */
bool GridPosition::operator ==(const GridPosition& position) const {
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
		} else if (label == TORCH) {
			parseTorchData();
		} else if (label == CHEST) {
			parseChestData();
		} else if (label == DOOR) {
			parseDoorData();
		} else if (label == DESECRATION) {
			parseDesecrationData();
		} else if (label == LAYER) {
			parseLayerData();
		}

		nextLine();
	}
}

LevelLoader::~LevelLoader() {
	levelData.layers.clear();
	levelData.torches.clear();
	levelData.chests.clear();
	levelData.doors.clear();
	levelData.desecrations.clear();
}

MathUtils::Axis LevelLoader::getAxis(int code) {
	return code == 0 ? MathUtils::Axis::Z : MathUtils::Axis::X;
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

void LevelLoader::parseChestData() {
	setChunkDelimiter(",");

	ChestData data;

	data.position.layer = std::stoi(readNextChunk());
	data.position.x = std::stoi(readNextChunk());
	data.position.z = std::stoi(readNextChunk());
	data.direction = getDirection(std::stoi(readNextChunk()));
	data.itemType = std::stoi(readNextChunk());

	levelData.chests.push_back(data);
}

void LevelLoader::parseDesecrationData() {
	setChunkDelimiter(",");

	DesecrationData data;

	data.position.layer = std::stoi(readNextChunk());
	data.position.x = std::stoi(readNextChunk());
	data.position.z = std::stoi(readNextChunk());

	levelData.desecrations.push_back(data);
}

void LevelLoader::parseDoorData() {
	setChunkDelimiter(",");

	DoorData data;

	data.position.layer = std::stoi(readNextChunk());
	data.position.x = std::stoi(readNextChunk());
	data.position.z = std::stoi(readNextChunk());
	data.axis = getAxis(std::stoi(readNextChunk()));

	levelData.doors.push_back(data);
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

void LevelLoader::parseTorchData() {
	setChunkDelimiter(",");

	TorchData data;

	data.position.layer = std::stoi(readNextChunk());
	data.position.x = std::stoi(readNextChunk());
	data.position.z = std::stoi(readNextChunk());
	data.direction = getDirection(std::stoi(readNextChunk()));

	levelData.torches.push_back(data);
}

void LevelLoader::parseVisibility() {
	levelData.visibility = std::stoi(readNextChunk());
}
