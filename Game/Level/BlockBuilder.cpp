#include <Level/BlockBuilder.h>
#include <Level/LevelLayout.h>
#include <GameConstants.h>
#include <GameObjects.h>
#include <MathUtils.h>
#include <SoftEngine.h>
#include <cmath>

/**
 * BlockBuilder
 * ------------
 */
BlockBuilder::BlockBuilder(LevelLayout* levelLayout) {
	this->levelLayout = levelLayout;

	blocksPerLayer = levelLayout->getSize().width * levelLayout->getSize().height;
	totalBlocks = levelLayout->getTotalLayers() * blocksPerLayer;
}

Block BlockBuilder::getNextBlock() {
	using namespace GameConstants;

	Block block;

	int layerIndex = (int)floor((float)blockCounter / blocksPerLayer);
	int blockIndex = blockCounter - blocksPerLayer * layerIndex;
	int x = blockIndex % levelLayout->getSize().width;
	int z = (int)floor(blockIndex / levelLayout->getSize().width);

	block.type = levelLayout->getBlockType(layerIndex, x, z);

	if (isSolid(block.type)) {
		int sidesMask = getBlockSidesMask(layerIndex, x, z);

		if (sidesMask == 0) {
			// Ignore blocks without any visible sides,
			// since no vertices/faces will be generated
			block.object = nullptr;
		} else {
			block.object = new SidedBlock(sidesMask);
		}
	} else {
		block.object = getBlockObject(block.type);
	}

	if (block.object != nullptr) {
		block.object->isStatic = true;
		block.object->scale(HALF_TILE_SIZE);

		block.object->position = {
			x * TILE_SIZE,
			layerIndex * TILE_SIZE - HALF_TILE_SIZE,
			-z * TILE_SIZE
		};
	}

	blockCounter++;

	return block;
}

Soft::Object* BlockBuilder::getBlockObject(int blockType) {
	using namespace GameConstants;

	Soft::Object* object = nullptr;

	switch (blockType) {
		case BlockTypes::COLUMN_MIDDLE:
			Soft::ObjLoader loader("./Assets/Models/column.obj");

			object = new Soft::Model(loader);
			object->setColor({ 150, 100, 50 });
			object->isFlatShaded = true;
			break;
	}

	return object;
}

int BlockBuilder::getBlockSidesMask(int layerIndex, int x, int z) {
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

bool BlockBuilder::hasBlocksRemaining() {
	return blockCounter < totalBlocks;
}

bool BlockBuilder::isSolid(int blockType) {
	return blockType >= GameConstants::BlockTypes::SOLID_1 && blockType <= GameConstants::BlockTypes::SOLID_2;
}

bool BlockBuilder::isVisibleSpace(int blockType) {
	return !isSolid(blockType) && blockType != GameConstants::BlockTypes::OUT_OF_BOUNDS;
}
