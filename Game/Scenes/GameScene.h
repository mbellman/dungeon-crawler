#pragma once

#include <SoftEngine.h>

enum TileDirection {
	UP,
	DOWN,
	LEFT,
	RIGHT
};

class GameScene : public Soft::Scene {
public:
	void load() override;
	void onUpdate(int dt) override;

private:
	constexpr static float TILE_SIZE = 250.0f;
	constexpr static int MOVE_STEP_DURATION = 250;

	TileDirection getYawTileDirection(float yaw);
	bool isMoving();
	void move(TileDirection direction);
};
