#pragma once

#include <Level/LevelLoader.h>
#include <Entities/Chest.h>
#include <Entities/Door.h>
#include <Entities/Desecration.h>
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

	void addChest(Chest* chest);
	void addDesecration(Desecration* desecration);
	void addDoor(Door* door);
	int getBlockType(int layerIndex, int x, int z) const;
	int getBlockType(const GridPosition& position) const;
	const Soft::Area& getSize() const;
	int getTotalLayers() const;
	Chest* getMatchingChest(const GridPosition& position) const;
	Desecration* getMatchingDesecration(const GridPosition& position) const;
	Door* getMatchingDoor(const GridPosition& position) const;
	bool hasImpassableObject(const GridPosition& position) const;
	bool isDesecrated(const GridPosition& position) const;
	bool isEmptyBlock(int layerIndex, int x, int z) const;
	bool isEmptyBlock(const GridPosition& position) const;
	bool isStaircaseBlock(int layerIndex, int x, int z) const;
	bool isStaircaseBlock(const GridPosition& position) const;
	bool isWalkableBlock(int layerIndex, int x, int z) const;
	bool isWalkableBlock(const GridPosition& position) const;
	bool isWalkablePosition(const GridPosition& position) const;
	void setBlockType(int layerIndex, int x, int z, int blockType);

private:
	int totalLayers = 0;
	Layer* layers = nullptr;
	std::vector<Chest*> chests;
	std::vector<Door*> doors;
	std::vector<Desecration*> desecrations;
};
