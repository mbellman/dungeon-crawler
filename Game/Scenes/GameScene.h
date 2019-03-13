#pragma once

#include <SoftEngine.h>
#include <MathUtils.h>
#include <Level/LevelLayout.h>
#include <Level/Floors.h>

class GameScene : public Soft::Scene {
public:
	~GameScene();

	void load() override;
	void onUpdate(int dt) override;

private:
	LevelLayout* levelLayout = nullptr;
	Position currentBlockPosition;

	bool canMoveInDirection(MathUtils::Direction direction);
	MathUtils::Direction getYawDirection(float yaw);
	bool isMoving();
	void loadLevel();
	void move(MathUtils::Direction direction);
	void spawn(const SpawnPosition& spawnPosition);
};
