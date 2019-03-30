#pragma once

#include <Level/LevelLoader.h>
#include <SoftEngine.h>
#include <MathUtils.h>
#include <vector>

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

	void addActionable(const Actionable& actionable);
	int getBlockType(int layerIndex, int x, int z) const;
	int getBlockType(GridPosition position) const;
	const Soft::Area& getSize() const;
	int getTotalLayers() const;
	const Actionable* getMatchingActionable(GridPosition position, MathUtils::Direction direction) const;
	bool isEmptyBlock(int layerIndex, int x, int z) const;
	bool isEmptyBlock(GridPosition position) const;
	bool isStaircaseBlock(int layerIndex, int x, int z) const;
	bool isStaircaseBlock(GridPosition position) const;
	bool isWalkableBlock(int layerIndex, int x, int z) const;
	bool isWalkableBlock(GridPosition position) const;
	bool isWalkablePosition(GridPosition position) const;
	void setBlockType(int layerIndex, int x, int z, int blockType);

private:
	int totalLayers = 0;
	int startingLayerIndex = 0;
	Soft::Coordinate startingBlockCoordinate;
	MathUtils::Direction startingDirection = MathUtils::Direction::UP;
	Layer* layers = nullptr;
	std::vector<Actionable> actionables;
};
