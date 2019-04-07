#include <Entities/HUD.h>
#include <GameUtils.h>

/**
 * HUD
 * ---
 */
HUD::HUD(const Soft::Area& windowArea) {
	this->windowArea = windowArea;
}

void HUD::initialize() {
	int baseHeight = windowArea.height - (windowArea.height * GameUtils::RASTER_REGION.height / 100.0f);
	int baseY = windowArea.height - baseHeight;

	Soft::UIGraphic* leftColumn = new Soft::UIGraphic("./Assets/UI/column.png");
	leftColumn->position = { -30, 0 };

	Soft::UIGraphic* rightColumn = new Soft::UIGraphic("./Assets/UI/column.png");
	rightColumn->position = { windowArea.width - rightColumn->getWidth() + 30, 0 };

	Soft::UIGraphic* base = new Soft::UIGraphic("./Assets/UI/hud.png");
	base->position = { 0, baseY };

	lightBar = new Soft::UIRect();
	lightBar->setSize(200, 24);
	lightBar->setColor({ 0, 0, 255 });
	lightBar->setAlpha(0.5f);
	lightBar->position = { 92, baseY + 27 };

	add(leftColumn);
	add(rightColumn);
	add(base);
	add(lightBar);
}

void HUD::onUpdate(int dt) {
	lightBar->clip(lightBar->getWidth() * getCastLightCooldownProgress(), lightBar->getHeight());
}

float HUD::getCastLightCooldownProgress() const {
	if (lastCastLightTime == 0) {
		return 1.0f;
	}

	float progress = (float)(getAge() - lastCastLightTime) / GameUtils::CAST_LIGHT_COOLDOWN_TIME;

	return progress > 1.0f ? 1.0f : progress;
}

void HUD::trackCastLightTime() {
	lastCastLightTime = getAge();
}
