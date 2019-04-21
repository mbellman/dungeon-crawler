#pragma once

#include <Level/LevelLoader.h>
#include <Entities/Chest.h>
#include <Entities/Door.h>
#include <Entities/Desecration.h>
#include <Entities/Interactible.h>
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

	template<typename T>
	void addInteractible(Interactible<T>* interactible) {
		interactibles.push_back((Interactible<>*)interactible);
	}

	int getBlockType(int layerIndex, int x, int z) const;
	int getBlockType(const GridPosition& position) const;
	const Soft::Area& getSize() const;
	int getTotalLayers() const;

	template<class T>
	T* getMatchingInteractible(const GridPosition& position, bool t = false) const {
		for (auto* interactible : interactibles) {
			if ((Soft::Entity*)interactible->isOfType<T>() && ((T*)interactible)->getData().position == position) {
				return (T*)interactible;
			}
		}

		return nullptr;
	}

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
	std::vector<Interactible<>*> interactibles;
};
