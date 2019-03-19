#pragma once

#include <Level/LevelLayout.h>
#include <GameConstants.h>
#include <SoftEngine.h>

struct Block {
	int type = GameConstants::BlockTypes::OUT_OF_BOUNDS;
	Soft::Object* object = nullptr;
};

class BlockBuilder {
public:
	BlockBuilder(LevelLayout* levelLayout);

	Block getNextBlock();
	bool hasBlocksRemaining();

private:
	const LevelLayout* levelLayout = nullptr;
	int blockCounter = 0;
	int totalBlocks = 0;
	int blocksPerLayer = 0;

	int getBlockSidesMask(int layerIndex, int x, int z);
	Soft::Object* getBlockObject(int blockType);
	bool isSolid(int blockType);
	bool isVisibleSpace(int blockType);
};
