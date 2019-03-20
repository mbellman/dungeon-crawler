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
	MathUtils::Direction staircaseDirection;

	void addCameraLight();
	void castLight();
	Soft::TextureBuffer* getBlockTexture(int blockType);
	float getCastLightCooldownProgress();
	GridPosition getDirectionalGridPosition(MathUtils::Direction direction);
	Soft::Vec3 getGridPositionVec3(GridPosition position);
	MathUtils::Direction getYawDirection(float yaw);
	bool isPlayerMoving();
	bool isPlayerOnStaircase();
	bool isWalkablePosition(GridPosition position);
	bool isWalkableUpStaircase(MathUtils::Direction direction);
	bool isWalkableDownStaircase(MathUtils::Direction direction);
	void loadLevel();
	void loadTextures();
	void loadUI();
	void move(MathUtils::Direction direction);
	void moveOffStaircase(MathUtils::Direction direction);
	void moveUpOntoStaircase(MathUtils::Direction direction);
	void moveDownOntoStaircase(MathUtils::Direction direction);
	void showText(const char* value);
	void spawnPlayer(const SpawnPosition& spawnPosition);
	void updateUI(int dt);
};
