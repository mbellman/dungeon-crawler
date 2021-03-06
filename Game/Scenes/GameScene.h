#pragma once

#include <SoftEngine.h>
#include <MathUtils.h>
#include <Inventory.h>
#include <Party.h>
#include <Level/LevelLayout.h>
#include <Level/LevelLoader.h>
#include <Entities/Chest.h>
#include <Entities/Door.h>
#include <SDL_ttf.h>

class GameScene : public Soft::Scene {
public:
	GameScene();
	~GameScene();

	void load() override;
	void onUpdate(int dt) override;

private:
	constexpr static int BATTLE_TRANSITION_DURATION = 1500;

	Party* party = nullptr;
	Inventory* inventory = nullptr;
	LevelLayout* levelLayout = nullptr;
	TTF_Font* uiFont = nullptr;
	SpawnPosition playerSpawnPosition;
	Soft::TweenableFloat battleTransitionProgress = 0.0f;

	void addBattleTransitionFade();
	void addHUD();
	void addPlayer();
	void addStaff();
	void castLight();
	Soft::TextureBuffer* getBlockTexture(int blockType);
	void handleAction();
	void handleChestAction(Chest* chest);
	void handleDoorAction(Door* door);
	void handleGameKeyDown(const SDL_Keycode& code);
	void handleItemMenuKeyDown(const SDL_Keycode& code);
	void initiateBattleTransition();
	bool isItemMenuOpen();
	void loadLevel();
	void loadParty();
	void loadTextures();
	bool shouldInitiateBattleTransition();
	void showItemMenu();
	void showItemObtainedText(const char* itemName);
	void showText(const char* value);
	void updateBattleTransition(int dt);
};
