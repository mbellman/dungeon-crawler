#pragma once

#include <Level/BlockBuilder.h>
#include <Level/LevelLayout.h>
#include <MathUtils.h>
#include <SoftEngine.h>

struct SpawnPosition {
	int layerIndex;
	int x;
	int z;
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
	extern FloorPlan<3, 10, 10> Floor1;
}
