#pragma once

#include <SoftEngine.h>

enum BlockTypes {
	EMPTY = 0,
	GROUND = 1,
	CEILING = 2,
	WALL_LEFT = 3,
	WALL_RIGHT = 4,
	WALL_FRONT = 5,
	WALL_BACK = 6,
	CUBE = 7
};

class BlockBuilder {
public:
	static Soft::Object* createBlockObject(int blockType);

private:
	static Soft::Mesh* createSingleTileMesh();
};
