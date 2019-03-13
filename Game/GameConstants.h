#pragma once

namespace GameConstants {
	constexpr static float TILE_SIZE = 250.0f;
	constexpr static float HALF_TILE_SIZE = TILE_SIZE / 2.0f;
	constexpr static int MOVE_STEP_DURATION = 250;

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
}
