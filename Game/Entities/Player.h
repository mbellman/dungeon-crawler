#pragma once

#include <MathUtils.h>
#include <Level/LevelLoader.h>
#include <Level/LevelLayout.h>
#include <SoftEngine.h>

class Player : public Soft::Entity {
public:
	Player(Soft::Camera* camera, const LevelLayout* levelLayout);

	void initialize() override;
	void onUpdate(int dt) override;
	const GridPosition& getCurrentGridPosition() const;
	MathUtils::Direction getDirection() const;
	void move(MathUtils::Direction direction);
	void spawn(const SpawnPosition& spawnPosition);

private:
	Soft::Camera* camera = nullptr;
	const LevelLayout* levelLayout = nullptr;
	GridPosition currentGridPosition;
	int lastMovementTime = 0;
	bool shouldBobCamera = false;

	void bobCamera();
	bool canWalkUpStaircase(MathUtils::Direction direction);
	bool canWalkDownStaircase(MathUtils::Direction direction);
	GridPosition getDirectionalGridPosition(MathUtils::Direction direction);
	bool isMoving();
	void moveOffStaircase(MathUtils::Direction direction);
	void moveUpOntoStaircase(MathUtils::Direction direction);
	void moveDownOntoStaircase(MathUtils::Direction direction);
};
