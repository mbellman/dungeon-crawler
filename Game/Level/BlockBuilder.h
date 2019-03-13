#pragma once

#include <Level/LevelLayout.h>
#include <SoftEngine.h>

class BlockBuilder {
public:
	BlockBuilder(LevelLayout* levelLayout);

	Soft::Object* getNextBlockObject();
	bool isComplete();

private:
	const LevelLayout* levelLayout = nullptr;
	int blockCounter = 0;
	int totalBlocks = 0;
	int blocksPerLayer = 0;

	int getBlockSides(int layerIndex, int x, int z);
	bool isVisibleSpace(int blockType);
};
