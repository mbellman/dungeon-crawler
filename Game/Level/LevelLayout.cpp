#include <Level/LevelLayout.h>
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
		layers[i].blocks = new Block[area.width * area.height];
	}
}

LevelLayout::~LevelLayout() {
	delete[] layers;
}

Block* LevelLayout::getBlock(int layerIndex, int x, int z) const {
	if (layerIndex >= totalLayers) {
		return nullptr;
	}

	Layer& layer = layers[layerIndex];

	if (x >= layer.area.width || z >= layer.area.height) {
		return nullptr;
	}

	int blockIndex = z * layer.area.width + x;

	return &layer.blocks[blockIndex];
}

const Soft::Area& LevelLayout::getSize() const {
	return layers[0].area;
}

int LevelLayout::getTotalLayers() const {
	return totalLayers;
}

void LevelLayout::setBlock(int layerIndex, int x, int z, const Block& block) {
	Block* levelBlock = getBlock(layerIndex, x, z);

	levelBlock->type = block.type;
	levelBlock->traversableDirections = block.traversableDirections;
}
