#pragma once

#include <Level/LevelLoader.h>
#include <SoftEngine.h>

namespace GameUtils {
	constexpr static float TILE_SIZE = 250.0f;
	constexpr static float HALF_TILE_SIZE = TILE_SIZE / 2.0f;
	constexpr static int MOVE_STEP_DURATION = 300;
	constexpr static int CAST_LIGHT_LIFETIME = 5000;
	constexpr static int CAST_LIGHT_COOLDOWN_TIME = 3000;
	constexpr static float CAST_LIGHT_RANGE = 800.0f;

	constexpr static Soft::Region RASTER_REGION = {
		5, 0, 90, 85
	};

	enum BlockTypes {
		OUT_OF_BOUNDS = -1,
		EMPTY = 0,
		SOLID_1 = 1,
		SOLID_2 = 2,
		COLUMN_BOTTOM = 3,
		COLUMN_MIDDLE = 4,
		COLUMN_TOP = 5,
		STAIRCASE_FORWARD = 6,
		STAIRCASE_BACKWARD = 7,
		STAIRCASE_LEFT = 8,
		STAIRCASE_RIGHT = 9,
		BRIDGE = 10
	};

	MathUtils::Direction getYawDirection(float yaw);
	Soft::Vec3 getGridPositionVec3(GridPosition position);
}