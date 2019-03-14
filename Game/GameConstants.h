#pragma once

namespace GameConstants {
	constexpr static float TILE_SIZE = 250.0f;
	constexpr static float HALF_TILE_SIZE = TILE_SIZE / 2.0f;
	constexpr static int MOVE_STEP_DURATION = 250;
	constexpr static int CAST_LIGHT_LIFETIME = 5000;
	constexpr static float CAST_LIGHT_RANGE = 800.0f;

	enum BlockTypes {
		OUT_OF_BOUNDS = -1,
		EMPTY = 0,
		WALL = 1
	};
}
