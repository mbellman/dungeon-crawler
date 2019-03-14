#pragma once

#include <SoftEngine.h>
#include <MathUtils.h>
#include <Level/LevelLayout.h>
#include <Level/LevelLoader.h>

class GameScene : public Soft::Scene {
public:
	~GameScene();

	void load() override;
	void onUpdate(int dt) override;

private:
	LevelLayout* levelLayout = nullptr;
	GridPosition currentGridPosition;
	int lastLightCastTime = 0;

	bool canMoveInDirection(MathUtils::Direction direction);
	void castLight();
	MathUtils::Direction getYawDirection(float yaw);
	bool isMoving();
	void loadLevel();
	void move(MathUtils::Direction direction);
	void spawn(const SpawnPosition& spawnPosition);
};
