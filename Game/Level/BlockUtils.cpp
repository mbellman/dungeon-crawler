#include <Level/BlockUtils.h>
#include <GameUtils.h>
#include <vector>

/**
 * BlockUtils
 * ----------
 */
std::vector<int> BlockUtils::walkableBlockTypes = {
	GameUtils::BlockTypes::SOLID_1,
	GameUtils::BlockTypes::SOLID_2,
	GameUtils::BlockTypes::BRIDGE
};

bool BlockUtils::isEmpty(int blockType) {
	return blockType == GameUtils::BlockTypes::EMPTY;
}

bool BlockUtils::isSolid(int blockType) {
	return blockType >= GameUtils::BlockTypes::SOLID_1 && blockType <= GameUtils::BlockTypes::SOLID_2;
}

bool BlockUtils::isStaircase(int blockType) {
	return blockType >= GameUtils::BlockTypes::STAIRCASE_FORWARD && blockType <= GameUtils::BlockTypes::STAIRCASE_RIGHT;
}

bool BlockUtils::isVisibleSpace(int blockType) {
	return !isSolid(blockType) && blockType != GameUtils::BlockTypes::OUT_OF_BOUNDS;
}

bool BlockUtils::isWalkable(int blockType) {
	for (int type : walkableBlockTypes) {
		if (blockType == type) {
			return true;
		}
	}

	return false;
}
