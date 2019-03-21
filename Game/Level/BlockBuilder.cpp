#include <Level/BlockBuilder.h>
#include <Level/LevelLayout.h>
#include <GameConstants.h>
#include <GameObjects.h>
#include <MathUtils.h>
#include <Level/BlockUtils.h>
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

	if (BlockUtils::isSolid(block.type)) {
		int sidesMask = getBlockSidesMask(layerIndex, x, z);

		if (sidesMask > 0) {
			// Only generate Objects for solids with visible sides,
			// since no vertices/faces will be generated otherwise
			block.object = new SidedBlock(sidesMask);
		}
	} else {
		block.object = getBlockObject(block.type);
	}

	if (block.object != nullptr) {
		block.object->isStatic = true;

		if (block.type != BlockTypes::BRIDGE) {
			block.object->scale(HALF_TILE_SIZE);
		}

		Soft::Vec3 offset = {
			x * TILE_SIZE,
			layerIndex * TILE_SIZE - HALF_TILE_SIZE,
			-z * TILE_SIZE
		};

		block.object->position += offset;
	}

	blockCounter++;

	return block;
}

Soft::Object* BlockBuilder::getBlockObject(int blockType) {
	using namespace GameConstants;

	Soft::Object* object = nullptr;

	switch (blockType) {
		case BlockTypes::COLUMN_BOTTOM: {
			Soft::ObjLoader loader("./Assets/Models/column-base.obj");

			object = new Soft::Model(loader);
			object->isFlatShaded = true;
			break;
		}
		case BlockTypes::COLUMN_MIDDLE: {
			Soft::ObjLoader loader("./Assets/Models/column.obj");

			object = new Soft::Model(loader);
			object->isFlatShaded = true;
			break;
		}
		case BlockTypes::COLUMN_TOP: {
			Soft::ObjLoader loader("./Assets/Models/column-base.obj");

			object = new Soft::Model(loader);
			object->rotateDeg({ 180.0f, 0.0f, 0.0f });
			object->isFlatShaded = true;
			break;
		}
		case BlockTypes::STAIRCASE_FORWARD: {
			Soft::ObjLoader loader("./Assets/Models/staircase.obj");

			object = new Soft::Model(loader);
			object->isFlatShaded = true;
			break;
		}
		case BlockTypes::STAIRCASE_BACKWARD: {
			Soft::ObjLoader loader("./Assets/Models/staircase.obj");

			object = new Soft::Model(loader);
			object->rotateDeg({ 0.0f, 180.0f, 0.0f });
			object->isFlatShaded = true;
			break;
		}
		case BlockTypes::STAIRCASE_LEFT: {
			Soft::ObjLoader loader("./Assets/Models/staircase.obj");

			object = new Soft::Model(loader);
			object->rotateDeg({ 0.0f, -90.0f, 0.0f });
			object->isFlatShaded = true;
			break;
		}
		case BlockTypes::STAIRCASE_RIGHT: {
			Soft::ObjLoader loader("./Assets/Models/staircase.obj");

			object = new Soft::Model(loader);
			object->rotateDeg({ 0.0f, 90.0f, 0.0f });
			object->isFlatShaded = true;
			break;
		}
		case BlockTypes::BRIDGE: {
			// Bridge blocks should not be scaled, but instead defined
			// with dimensions at instantiation, owing to how Billboards
			// separate their sides very slightly to avoid z-fighting.
			object = new Soft::Billboard(TILE_SIZE, TILE_SIZE);
			object->rotateDeg({ 90.0f, 0.0f, 0.0f });
			object->position.y -= HALF_TILE_SIZE;
			object->canOccludeSurfaces = false;
			break;
		}
	}

	return object;
}

int BlockBuilder::getBlockSidesMask(int layerIndex, int x, int z) {
	using namespace MathUtils;
	using namespace BlockUtils;

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
