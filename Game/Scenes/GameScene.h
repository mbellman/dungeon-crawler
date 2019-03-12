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
	constexpr static float DEG_45 = M_PI / 4.0f;
	constexpr static float DEG_90 = DEG_45 * 2.0f;
	constexpr static float DEG_135 = DEG_45 * 3.0f;
	constexpr static float DEG_180 = DEG_45 * 4.0f;
	constexpr static float DEG_225 = DEG_45 * 5.0f;
	constexpr static float DEG_270 = DEG_45 * 6.0f;
	constexpr static float DEG_315 = DEG_45 * 7.0f;
	constexpr static float DEG_360 = DEG_45 * 8.0f;

	constexpr static float TILE_SIZE = 150.0f;
	constexpr static int MOVE_STEP_DURATION = 500;

	float getWrappedYaw(float yaw);
	TileDirection getYawTileDirection(float yaw);
	bool isMoving();
	void move(TileDirection direction);
	void onKeyDown(const SDL_Keycode& code);
};
