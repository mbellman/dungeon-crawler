#include <GameUtils.h>
#include <MathUtils.h>
#include <Level/LevelLoader.h>
#include <SoftEngine.h>

/**
 * GameUtils
 * ---------
 */
MathUtils::Direction GameUtils::getYawDirection(float yaw) {
	using namespace MathUtils;

	float wrappedYaw = modf(yaw, DEG_360);

	if (wrappedYaw < DEG_45 || wrappedYaw >= DEG_315) {
		return Direction::FORWARD;
	} else if (wrappedYaw < DEG_135 && wrappedYaw >= DEG_45) {
		return Direction::LEFT;
	} else if (wrappedYaw < DEG_225 && wrappedYaw >= DEG_135) {
		return Direction::BACKWARD;
	} else if (wrappedYaw < DEG_315 && wrappedYaw >= DEG_225) {
		return Direction::RIGHT;
	}

	return Direction::UP;
}

Soft::Vec3 GameUtils::getGridPositionVec3(GridPosition position) {
	return {
		position.x * TILE_SIZE,
		position.layer * TILE_SIZE - HALF_TILE_SIZE,
		-position.z * TILE_SIZE
	};
}
