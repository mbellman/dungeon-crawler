#pragma once

#include <Level/LevelLoader.h>
#include <SoftEngine.h>
#include <MathUtils.h>

/**
 * Layer
 * -----
 */
struct Layer {
	int* blocks = nullptr;
	Soft::Area area;

	~Layer();
};

/**
 * LevelLayout
 * -----------
 */
class LevelLayout {
public:
	LevelLayout(int totalLayers, const Soft::Area& area);
	~LevelLayout();

	int getBlockType(int layerIndex, int x, int z) const;
	int getBlockType(GridPosition position) const;
	const Soft::Area& getSize() const;
	int getTotalLayers() const;
	bool isEmptyBlock(int layerIndex, int x, int z) const;
	bool isEmptyBlock(GridPosition position) const;
	bool isStaircaseBlock(int layerIndex, int x, int z) const;
	bool isStaircaseBlock(GridPosition position) const;
	bool isWalkableBlock(int layerIndex, int x, int z) const;
	bool isWalkableBlock(GridPosition position) const;
	void setBlockType(int layerIndex, int x, int z, int blockType);

private:
	int totalLayers = 0;
	int startingLayerIndex = 0;
	Soft::Coordinate startingBlockCoordinate;
	MathUtils::Direction startingDirection = MathUtils::Direction::UP;
	Layer* layers = nullptr;
};
