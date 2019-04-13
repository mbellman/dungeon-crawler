#pragma once

#include <SoftEngine.h>
#include <Party.h>

class PartyMemberHUD : public Soft::Entity {
public:
	PartyMemberHUD(const PartyMember* partyMember, int index);

	void initialize() override;
	void onUpdate(int dt) override;

private:
	static Soft::Region HEALTH_BAR_REGION;
	static Soft::Region MAGIC_BAR_REGION;
	static Soft::Coordinate positions[4];

	const PartyMember* partyMember = nullptr;
	Soft::UIRect* healthBar = nullptr;
	Soft::UIRect* magicBar = nullptr;
	int index;
};

class HUD : public Soft::Entity {
public:
	HUD(const Soft::Area& windowArea, const Party* party);

	void initialize() override;
	void onUpdate(int dt) override;
	float getCastLightCooldownProgress() const;
	void trackCastLightTime();

private:
	Soft::Area windowArea;
	Soft::UIRect* lightBar = nullptr;
	const Party* party = nullptr;
	int lastCastLightTime = 0;
};
