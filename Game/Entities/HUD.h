#pragma once

#include <SoftEngine.h>

class HUD : public Soft::Entity {
public:
	HUD(const Soft::Area& windowArea);

	void initialize() override;
	void onUpdate(int dt) override;
	float getCastLightCooldownProgress() const;
	void trackCastLightTime();

private:
	Soft::Area windowArea;
	Soft::UIRect* lightBar = nullptr;
	int lastCastLightTime = 0;
};
