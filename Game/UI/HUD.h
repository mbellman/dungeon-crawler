#pragma once

#include <SoftEngine.h>
#include <Party.h>
#include <SDL_ttf.h>

class PartyMemberHUD : public Soft::Entity {
public:
	PartyMemberHUD(const PartyMember* partyMember, int index, TTF_Font* font);

	void initialize() override;
	void onUpdate(int dt) override;

private:
	static Soft::Region HEALTH_BAR_REGION;
	static Soft::Region MAGIC_BAR_REGION;
	static Soft::Coordinate positions[4];

	TTF_Font* font = nullptr;
	const PartyMember* partyMember = nullptr;
	Soft::UIRect* healthBar = nullptr;
	Soft::UIRect* magicBar = nullptr;
	int index;

	void addBars();
	void addIcon();
	void addName();
	const Soft::Coordinate& getContainerOffset();
};

class HUD : public Soft::Entity {
public:
	HUD(const Party* party);

	void initialize() override;
	void onUpdate(int dt) override;
	float getCastLightCooldownProgress() const;
	void trackCastLightTime();

private:
	TTF_Font* font = nullptr;
	Soft::UIRect* lightBar = nullptr;
	const Party* party = nullptr;
	int lastCastLightTime = 0;
};
