#include <Level/BlockBuilder.h>
#include <Level/LevelLayout.h>
#include <GameConstants.h>
#include <GameObjects.h>
#include <MathUtils.h>
#include <SoftEngine.h>
#include <cmath>

BlockBuilder::BlockBuilder(LevelLayout* levelLayout) {
	this->levelLayout = levelLayout;

	blocksPerLayer = levelLayout->getSize().width * levelLayout->getSize().height;
	totalBlocks = levelLayout->getTotalLayers() * blocksPerLayer;
}

Soft::Object* BlockBuilder::getNextBlockObject() {
	using namespace GameConstants;

	int layerIndex = (int)floor((float)blockCounter / blocksPerLayer);
	int blockIndex = blockCounter - blocksPerLayer * layerIndex;
	int x = blockIndex % levelLayout->getSize().width;
	int z = (int)floor(blockIndex / levelLayout->getSize().width);
	int blockType = levelLayout->getBlockType(layerIndex, x, z);

	blockCounter++;

	if (blockType == GameConstants::BlockTypes::WALL) {
		int sides = getBlockSides(layerIndex, x, z);

		SidedBlock* block = new SidedBlock(sides);

		block->scale(HALF_TILE_SIZE);

		block->position = {
			x * TILE_SIZE,
			layerIndex * TILE_SIZE - HALF_TILE_SIZE,
			-z * TILE_SIZE
		};

		return block;
	}

	return nullptr;
}

int BlockBuilder::getBlockSides(int layerIndex, int x, int z) {
	using namespace MathUtils;

	int sides = 0;
	int topBlock = levelLayout->getBlockType(layerIndex + 1, x, z);
	int bottomBlock = levelLayout->getBlockType(layerIndex - 1, x, z);
	int leftBlock = levelLayout->getBlockType(layerIndex, x - 1, z);
	int rightBlock = levelLayout->getBlockType(layerIndex, x + 1, z);
	int frontBlock = levelLayout->getBlockType(layerIndex, x, z - 1);
	int backBlock = levelLayout->getBlockType(layerIndex, x, z + 1);

	if (isVisibleSpace(topBlock)) {
		sides |= Direction::UP;
	}

	if (isVisibleSpace(bottomBlock)) {
		sides |= Direction::DOWN;
	}

	if (isVisibleSpace(leftBlock)) {
		sides |= Direction::LEFT;
	}

	if (isVisibleSpace(rightBlock)) {
		sides |= Direction::RIGHT;
	}

	if (isVisibleSpace(frontBlock)) {
		sides |= Direction::FORWARD;
	}

	if (isVisibleSpace(backBlock)) {
		sides |= Direction::BACKWARD;
	}

	return sides;
}

bool BlockBuilder::isComplete() {
	return blockCounter >= totalBlocks;
}

bool BlockBuilder::isVisibleSpace(int blockType) {
	return blockType != GameConstants::BlockTypes::WALL && blockType != GameConstants::BlockTypes::OUT_OF_BOUNDS;
}
