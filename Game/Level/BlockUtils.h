#pragma once

#include <vector>

namespace BlockUtils {
	extern std::vector<int> walkableBlockTypes;

	bool isEmpty(int blockType);
	bool isSolid(int blockType);
	bool isStaircase(int blockType);
	bool isVisibleSpace(int blockType);
	bool isWalkable(int blockType);
}
