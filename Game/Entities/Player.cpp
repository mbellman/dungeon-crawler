#include <Entities/Player.h>
#include <GameUtils.h>
#include <MathUtils.h>

/**
 * Player
 * ------
 */
Player::Player(Soft::Camera* camera, const LevelLayout* levelLayout) {
	this->camera = camera;
	this->levelLayout = levelLayout;
}

void Player::initialize() {
	desecrationWarningLight = new Soft::Light();

	desecrationWarningLight->setColor({ 255, 0, 0 });
	desecrationWarningLight->lockTo(camera);
	desecrationWarningLight->range = 2000.0f;
	desecrationWarningLight->power = 0.0f;

	add(desecrationWarningLight);
}

void Player::onUpdate(int dt) {
	if (isMoving() && shouldBobCamera) {
		bobCamera();
	}
}

void Player::bobCamera() {
	using namespace GameUtils;

	float yBasis = currentGridPosition.layer * TILE_SIZE - HALF_TILE_SIZE;
	float movementProgress = (float)(getAge() - lastMovementTime) / MOVE_STEP_DURATION;
	float yOffset = -4.0f * sinf(movementProgress * M_PI);

	camera->position.y = yBasis + yOffset;
}

bool Player::canWalkDownStaircase(MathUtils::Direction direction) {
	using namespace GameUtils;
	using namespace MathUtils;

	GridPosition targetGridPosition = getDirectionalGridPosition(direction);
	int targetFloorBlockType = levelLayout->getBlockType(targetGridPosition.layer - 1, targetGridPosition.x, targetGridPosition.z);

	return (
		(direction == Direction::FORWARD && targetFloorBlockType == BlockTypes::STAIRCASE_BACKWARD) ||
		(direction == Direction::BACKWARD && targetFloorBlockType == BlockTypes::STAIRCASE_FORWARD) ||
		(direction == Direction::LEFT && targetFloorBlockType == BlockTypes::STAIRCASE_RIGHT) ||
		(direction == Direction::RIGHT && targetFloorBlockType == BlockTypes::STAIRCASE_LEFT)
	);
}

bool Player::canWalkUpStaircase(MathUtils::Direction direction) {
	using namespace GameUtils;
	using namespace MathUtils;

	GridPosition targetGridPosition = getDirectionalGridPosition(direction);
	int targetPositionBlockType = levelLayout->getBlockType(targetGridPosition);

	return (
		(direction == Direction::FORWARD && targetPositionBlockType == BlockTypes::STAIRCASE_FORWARD) ||
		(direction == Direction::BACKWARD && targetPositionBlockType == BlockTypes::STAIRCASE_BACKWARD) ||
		(direction == Direction::LEFT && targetPositionBlockType == BlockTypes::STAIRCASE_LEFT) ||
		(direction == Direction::RIGHT && targetPositionBlockType == BlockTypes::STAIRCASE_RIGHT)
	);
}

const GridPosition& Player::getCurrentGridPosition() const {
	return currentGridPosition;
}

const Soft::Vec3& Player::getCurrentPosition() const {
	return camera->position;
}

MathUtils::Direction Player::getDirection() const {
	return GameUtils::getYawDirection(camera->yaw);
}

GridPosition Player::getDirectionalGridPosition(MathUtils::Direction direction) const {
	GridPosition targetGridPosition = currentGridPosition;

	switch (direction) {
		case MathUtils::Direction::FORWARD:
			targetGridPosition.z--;
			break;
		case MathUtils::Direction::BACKWARD:
			targetGridPosition.z++;
			break;
		case MathUtils::Direction::LEFT:
			targetGridPosition.x--;
			break;
		case MathUtils::Direction::RIGHT:
			targetGridPosition.x++;
			break;
	}

	return targetGridPosition;
}

bool Player::isMoving() {
	return camera->isTweening();
}

void Player::move(MathUtils::Direction direction) {
	if (isMoving()) {
		return;
	}

	GridPosition targetGridPosition = getDirectionalGridPosition(direction);
	shouldBobCamera = false;
	lastMovementTime = getAge();

	if (levelLayout->isStaircaseBlock(currentGridPosition)) {
		moveOffStaircase(direction);
	} else if (canWalkUpStaircase(direction)) {
		moveUpOntoStaircase(direction);
	} else if (canWalkDownStaircase(direction)) {
		moveDownOntoStaircase(direction);
	} else if (levelLayout->isWalkablePosition(targetGridPosition)) {
		Soft::Vec3 targetCameraPosition = GameUtils::getGridPositionVec3(targetGridPosition);

		camera->tweenTo(targetCameraPosition, GameUtils::MOVE_STEP_DURATION, Soft::Ease::linear);

		currentGridPosition = targetGridPosition;
		shouldBobCamera = true;
	}

	updateDesecrationWarningLight();
}

void Player::moveOffStaircase(MathUtils::Direction direction) {
	using namespace GameUtils;
	using namespace MathUtils;

	int blockType = levelLayout->getBlockType(currentGridPosition);
	GridPosition targetGridPosition = currentGridPosition;

	switch (blockType) {
		case BlockTypes::STAIRCASE_FORWARD:
			if (direction == Direction::FORWARD) {
				targetGridPosition.z--;
				targetGridPosition.layer++;
			} else if (direction == Direction::BACKWARD) {
				targetGridPosition.z++;
			}
			break;
		case BlockTypes::STAIRCASE_BACKWARD:
			if (direction == Direction::BACKWARD) {
				targetGridPosition.z++;
				targetGridPosition.layer++;
			} else if (direction == Direction::FORWARD) {
				targetGridPosition.z--;
			}
			break;
		case BlockTypes::STAIRCASE_LEFT:
			if (direction == Direction::LEFT) {
				targetGridPosition.x--;
				targetGridPosition.layer++;
			} else if (direction == Direction::RIGHT) {
				targetGridPosition.x++;
			}
			break;
		case BlockTypes::STAIRCASE_RIGHT:
			if (direction == Direction::RIGHT) {
				targetGridPosition.x++;
				targetGridPosition.layer++;
			} else if (direction == Direction::LEFT) {
				targetGridPosition.x--;
			}
			break;
	}

	if (targetGridPosition == currentGridPosition) {
		return;
	}

	Soft::Vec3 targetCameraPosition = getGridPositionVec3(targetGridPosition);

	if (levelLayout->isStaircaseBlock(targetGridPosition)) {
		targetCameraPosition.y += HALF_TILE_SIZE;
	} else if (levelLayout->isStaircaseBlock(targetGridPosition.layer - 1, targetGridPosition.x, targetGridPosition.z)) {
		targetCameraPosition.y -= HALF_TILE_SIZE;
		targetGridPosition.layer--;
	} else if (!levelLayout->isWalkablePosition(targetGridPosition)) {
		return;
	}

	camera->tweenTo(targetCameraPosition, MOVE_STAIRCASE_STEP_DURATION, Soft::Ease::linear);
	currentGridPosition = targetGridPosition;
}

void Player::moveDownOntoStaircase(MathUtils::Direction direction) {
	using namespace GameUtils;
	using namespace MathUtils;

	GridPosition targetGridPosition = getDirectionalGridPosition(direction);
	targetGridPosition.layer--;

	Soft::Vec3 targetCameraPosition = getGridPositionVec3(targetGridPosition);
	targetCameraPosition.y += HALF_TILE_SIZE;

	camera->tweenTo(targetCameraPosition, MOVE_STAIRCASE_STEP_DURATION, Soft::Ease::linear);
	currentGridPosition = targetGridPosition;
}

void Player::moveUpOntoStaircase(MathUtils::Direction direction) {
	using namespace GameUtils;
	using namespace MathUtils;

	GridPosition targetGridPosition = getDirectionalGridPosition(direction);
	Soft::Vec3 targetCameraPosition = getGridPositionVec3(targetGridPosition);
	targetCameraPosition.y += HALF_TILE_SIZE;

	camera->tweenTo(targetCameraPosition, MOVE_STAIRCASE_STEP_DURATION, Soft::Ease::linear);
	currentGridPosition = targetGridPosition;
}

void Player::spawn(const SpawnPosition& spawnPosition) {
	using namespace GameUtils;
	using namespace MathUtils;

	currentGridPosition.layer = spawnPosition.layer;
	currentGridPosition.x = spawnPosition.x;
	currentGridPosition.z = spawnPosition.z;

	camera->position = getGridPositionVec3(currentGridPosition);

	switch (spawnPosition.direction) {
		case Direction::FORWARD:
			camera->yaw = 0.0f;
			break;
		case Direction::LEFT:
			camera->yaw = DEG_90;
			break;
		case Direction::BACKWARD:
			camera->yaw = DEG_180;
			break;
		case Direction::RIGHT:
			camera->yaw = DEG_270;
			break;
	}
}

void Player::updateDesecrationWarningLight() {
	float updatedPower = levelLayout->isDesecrated(currentGridPosition) ? 1.0f : 0.0f;

	if (desecrationWarningLight->power.value() != updatedPower) {
		desecrationWarningLight->power.tweenTo(updatedPower, 500, Soft::Ease::linear);
	}
}
