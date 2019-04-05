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
	SpawnPosition playerSpawnPosition;
	int lastLightCastTime = 0;

	void addPlayer();
	void addStaff();
	void castLight();
	Soft::TextureBuffer* getBlockTexture(int blockType);
	float getCastLightCooldownProgress();
	void handleAction();
	void handleChestAction(Chest* chest);
	void handleKeyDown(const SDL_Keycode& code);
	void loadLevel();
	void loadTextures();
	void loadUI();
	void showItemObtainedText(const char* itemName);
	void showText(const char* value);
	void updateUI(int dt);
};
