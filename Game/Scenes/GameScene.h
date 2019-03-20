#pragma once

#include <SoftEngine.h>
#include <MathUtils.h>
#include <Level/LevelLayout.h>
#include <Level/LevelLoader.h>
#include <SDL_ttf.h>

class GameScene : public Soft::Scene {
public:
	GameScene();
	~GameScene();

	void load() override;
	void onUpdate(int dt) override;

private:
	LevelLayout* levelLayout = nullptr;
	TTF_Font* uiFont = nullptr;
	GridPosition currentGridPosition;
	int lastLightCastTime = 0;

	void addCameraLight();
	bool canMoveInDirection(MathUtils::Direction direction);
	void castLight();
	Soft::TextureBuffer* getBlockTexture(int blockType);
	float getCastLightCooldownProgress();
	MathUtils::Direction getYawDirection(float yaw);
	bool isMoving();
	void loadLevel();
	void loadTextures();
	void loadUI();
	void move(MathUtils::Direction direction);
	void showText(const char* value);
	void spawn(const SpawnPosition& spawnPosition);
	void updateUI(int dt);
};
