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

struct LightSpawn {
	Position position;
	Soft::Color color;
};

namespace Floor1 {
	extern int totalLayers;
	extern Soft::Area size;
	extern SpawnPosition spawnPosition;
	extern int blocks[4][20][20];
	extern int totalLights;
	extern LightSpawn lights[5];
}
