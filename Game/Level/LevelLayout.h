#pragma once

#include <SoftEngine.h>
#include <MathUtils.h>

/**
 * Block
 * -----
 */
struct Block {
	int type;
	int traversableDirections = 0;
};

/**
 * Layer
 * -----
 */
struct Layer {
	Block* blocks = nullptr;
	Soft::Area area;

	~Layer();
};

/**
 * LevelLayout
 * -----------
 */
class LevelLayout {
public:
	LevelLayout(int totalLayers, Soft::Area area);
	~LevelLayout();

	Block* getBlock(int layerIndex, int x, int z) const;
	const Soft::Area& getSize() const;
	int getTotalLayers() const;
	void setBlock(int layerIndex, int x, int z, const Block& block);

private:
	int totalLayers = 0;
	int startingLayerIndex = 0;
	Soft::Coordinate startingBlockCoordinate;
	MathUtils::Direction startingDirection = MathUtils::Direction::UP;
	Layer* layers = nullptr;
};