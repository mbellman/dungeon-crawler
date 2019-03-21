#include <Level/BlockUtils.h>
#include <GameConstants.h>
#include <vector>

/**
 * BlockUtils
 * ----------
 */
std::vector<int> BlockUtils::walkableBlockTypes = {
	GameConstants::BlockTypes::SOLID_1,
	GameConstants::BlockTypes::SOLID_2,
	GameConstants::BlockTypes::BRIDGE
};

bool BlockUtils::isEmpty(int blockType) {
	return blockType == GameConstants::BlockTypes::EMPTY;
}

bool BlockUtils::isSolid(int blockType) {
	return blockType >= GameConstants::BlockTypes::SOLID_1 && blockType <= GameConstants::BlockTypes::SOLID_2;
}

bool BlockUtils::isStaircase(int blockType) {
	return blockType >= GameConstants::BlockTypes::STAIRCASE_FORWARD && blockType <= GameConstants::BlockTypes::STAIRCASE_RIGHT;
}

bool BlockUtils::isVisibleSpace(int blockType) {
	return !isSolid(blockType) && blockType != GameConstants::BlockTypes::OUT_OF_BOUNDS;
}

bool BlockUtils::isWalkable(int blockType) {
	for (int type : walkableBlockTypes) {
		if (blockType == type) {
			return true;
		}
	}

	return false;
}
