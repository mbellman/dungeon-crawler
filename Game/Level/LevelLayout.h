#pragma once

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
	LevelLayout(int totalLayers, Soft::Area area);
	~LevelLayout();

	int getBlockType(int layerIndex, int x, int z) const;
	const Soft::Area& getSize() const;
	int getTotalLayers() const;
	void setBlockType(int layerIndex, int x, int z, int blockType);

private:
	int totalLayers = 0;
	int startingLayerIndex = 0;
	Soft::Coordinate startingBlockCoordinate;
	MathUtils::Direction startingDirection = MathUtils::Direction::UP;
	Layer* layers = nullptr;
};
