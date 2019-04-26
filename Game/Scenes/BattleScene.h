#pragma once

#include <SoftEngine.h>
#include <Inventory.h>
#include <Party.h>

class BattleScene : public Soft::Scene {
public:
	BattleScene(Party* party, Inventory* inventory);

	void load() override;

private:
	Party* party = nullptr;
	Inventory* inventory = nullptr;

	void addMenu();
	void addScenery();
	void loadTextures();
};
