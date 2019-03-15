#include <Level/LevelLayout.h>
#include <GameConstants.h>
#include <SoftEngine.h>

/**
 * Layer
 * -----
 */
Layer::~Layer() {
	delete[] blocks;
}

/**
 * LevelLayout
 * -----------
 */
LevelLayout::LevelLayout(int totalLayers, const Soft::Area& area) {
	this->totalLayers = totalLayers;

	layers = new Layer[totalLayers];

	for (int i = 0; i < totalLayers; i++) {
		layers[i].area = { area.width, area.height };
		layers[i].blocks = new int[area.width * area.height];
	}
}

LevelLayout::~LevelLayout() {
	delete[] layers;
}

int LevelLayout::getBlockType(int layerIndex, int x, int z) const {
	if (layerIndex < 0 || layerIndex >= totalLayers) {
		return GameConstants::BlockTypes::OUT_OF_BOUNDS;
	}

	Layer& layer = layers[layerIndex];

	if (x < 0 || x >= layer.area.width || z < 0 || z >= layer.area.height) {
		return GameConstants::BlockTypes::OUT_OF_BOUNDS;
	}

	int blockIndex = z * layer.area.width + x;

	return layer.blocks[blockIndex];
}

const Soft::Area& LevelLayout::getSize() const {
	return layers[0].area;
}

int LevelLayout::getTotalLayers() const {
	return totalLayers;
}

bool LevelLayout::isEmptyBlock(int layerIndex, int x, int z) const {
	return getBlockType(layerIndex, x, z) == GameConstants::BlockTypes::EMPTY;
}

bool LevelLayout::isWalkableBlock(int layerIndex, int x, int z) const {
	return getBlockType(layerIndex, x, z) >= GameConstants::BlockTypes::SOLID_1;
}

void LevelLayout::setBlockType(int layerIndex, int x, int z, int blockType) {
	if (layerIndex >= totalLayers) {
		return;
	}

	Layer* layer = &layers[layerIndex];
	int blockIndex = z * layer->area.width + x;

	layer->blocks[blockIndex] = blockType;
}
