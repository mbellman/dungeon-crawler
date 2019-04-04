#include <GameUtils.h>
#include <MathUtils.h>
#include <Level/LevelLoader.h>
#include <SoftEngine.h>

/**
 * GameUtils
 * ---------
 */
float GameUtils::getDirectionalRotationAngle(MathUtils::Direction direction) {
	switch (direction) {
		case MathUtils::Direction::LEFT:
			return 90.0f;
		case MathUtils::Direction::FORWARD:
			return 180.0f;
		case MathUtils::Direction::RIGHT:
			return 270.0f;
		default:
			return 0.0f;
	}
}

Soft::Vec3 GameUtils::getDirectionVector(MathUtils::Direction direction) {
	switch (direction) {
		case MathUtils::Direction::FORWARD:
			return { 0.0f, 0.0f, 1.0f };
		case MathUtils::Direction::BACKWARD:
			return { 0.0f, 0.0f, -1.0f };
		case MathUtils::Direction::LEFT:
			return { -1.0f, 0.0f, 0.0f };
		case MathUtils::Direction::RIGHT:
			return { 1.0f, 0.0f, 0.0f };
		case MathUtils::Direction::UP:
			return { 0.0f, 1.0f, 0.0f };
		case MathUtils::Direction::DOWN:
			return { 0.0f, -1.0f, 0.0f };
		default:
			return { 0.0f, 0.0f, 1.0f };
	}
}

Soft::Vec3 GameUtils::getGridPositionVec3(GridPosition position) {
	return {
		position.x * TILE_SIZE,
		position.layer * TILE_SIZE - HALF_TILE_SIZE,
		-position.z * TILE_SIZE
	};
}

MathUtils::Direction GameUtils::getYawDirection(float yaw) {
	using namespace MathUtils;

	float wrappedYaw = Soft::Math::modf(yaw, DEG_360);

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

void GameUtils::rotateToDirection(Soft::Object* object, MathUtils::Direction direction) {
	object->rotateDeg({
		0.0f,
		getDirectionalRotationAngle(direction),
		0.0f
	});
}
