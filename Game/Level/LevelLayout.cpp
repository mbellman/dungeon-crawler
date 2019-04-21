#include <Level/LevelLayout.h>
#include <Level/LevelLoader.h>
#include <Level/BlockUtils.h>
#include <Entities/Chest.h>
#include <Entities/Door.h>
#include <Entities/Desecration.h>
#include <GameUtils.h>
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

	interactibles.clear();
}

int LevelLayout::getBlockType(int layerIndex, int x, int z) const {
	if (layerIndex < 0 || layerIndex >= totalLayers) {
		return GameUtils::BlockTypes::OUT_OF_BOUNDS;
	}

	Layer& layer = layers[layerIndex];

	if (x < 0 || x >= layer.area.width || z < 0 || z >= layer.area.height) {
		return GameUtils::BlockTypes::OUT_OF_BOUNDS;
	}

	int blockIndex = z * layer.area.width + x;

	return layer.blocks[blockIndex];
}

int LevelLayout::getBlockType(const GridPosition& position) const {
	return getBlockType(position.layer, position.x, position.z);
}

const Soft::Area& LevelLayout::getSize() const {
	return layers[0].area;
}

int LevelLayout::getTotalLayers() const {
	return totalLayers;
}

bool LevelLayout::hasImpassableObject(const GridPosition& position) const {
	const Chest* chest = getMatchingInteractible<Chest>(position);
	const Door* door = getMatchingInteractible<Door>(position);

	return chest != nullptr || (door != nullptr && !door->isOpen());
}

bool LevelLayout::isDesecrated(const GridPosition& position) const {
	return getMatchingInteractible<Desecration>(position) != nullptr;
}

bool LevelLayout::isEmptyBlock(int layerIndex, int x, int z) const {
	return BlockUtils::isEmpty(getBlockType(layerIndex, x, z));
}

bool LevelLayout::isEmptyBlock(const GridPosition& position) const {
	return isEmptyBlock(position.layer, position.x, position.z);
}

bool LevelLayout::isStaircaseBlock(int layerIndex, int x, int z) const {
	return BlockUtils::isStaircase(getBlockType(layerIndex, x, z));
}

bool LevelLayout::isStaircaseBlock(const GridPosition& position) const {
	return isStaircaseBlock(position.layer, position.x, position.z);
}

bool LevelLayout::isWalkableBlock(int layerIndex, int x, int z) const {
	return BlockUtils::isWalkable(getBlockType(layerIndex, x, z));
}

bool LevelLayout::isWalkableBlock(const GridPosition& position) const {
	return isWalkableBlock(position.layer, position.x, position.z);
}

bool LevelLayout::isWalkablePosition(const GridPosition& position) const {
	bool isBridged = getBlockType(position) == GameUtils::BlockTypes::BRIDGE;

	return (
		!hasImpassableObject(position) &&
		(isEmptyBlock(position) || isBridged) &&
		(isWalkableBlock(position.layer - 1, position.x, position.z) || isBridged)
	);
}

void LevelLayout::setBlockType(int layerIndex, int x, int z, int blockType) {
	if (layerIndex >= totalLayers) {
		return;
	}

	Layer* layer = &layers[layerIndex];
	int blockIndex = z * layer->area.width + x;

	layer->blocks[blockIndex] = blockType;
}
