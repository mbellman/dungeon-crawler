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
LevelLayout::LevelLayout(int totalLayers, Soft::Area area) {
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
	if (layerIndex >= totalLayers) {
		return GameConstants::BlockTypes::EMPTY;
	}

	Layer& layer = layers[layerIndex];

	if (x >= layer.area.width || z >= layer.area.height) {
		return GameConstants::BlockTypes::EMPTY;
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

void LevelLayout::setBlockType(int layerIndex, int x, int z, int blockType) {
	if (layerIndex >= totalLayers) {
		return;
	}

	Layer* layer = &layers[layerIndex];
	int blockIndex = z * layer->area.width + x;

	layer->blocks[blockIndex] = blockType;
}
