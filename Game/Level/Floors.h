#pragma once

#include <Level/BlockBuilder.h>
#include <Level/LevelLayout.h>
#include <MathUtils.h>
#include <SoftEngine.h>

struct Position {
	int layer = 0;
	int x = 0;
	int z = 0;
};

struct SpawnPosition : Position {
	MathUtils::Direction direction;
};

template<int L, int W, int H>
struct FloorPlan {
	int totalLayers;
	Soft::Area size;
	SpawnPosition spawnPosition;
	int blocks[L][H][W];
};

namespace Floors {
	extern FloorPlan<4, 20, 20> Floor1;
}
