#include <Entities/HUD.h>
#include <GameUtils.h>
#include <SoftEngine.h>
#include <Party.h>
#include <string>

/**
 * PartyMemberHUD
 * --------------
 */
PartyMemberHUD::PartyMemberHUD(const PartyMember* partyMember, int index) {
	this->partyMember = partyMember;
	this->index = index;
}

void PartyMemberHUD::initialize() {
	Soft::Coordinate offset = PartyMemberHUD::positions[index];

	Soft::UIGraphic* slab = new Soft::UIGraphic("./Assets/UI/PartyHUD/slab.png");
	slab->position = offset;

	std::string iconPath = "./Assets/UI/PartyHUD/icon-" + std::to_string(partyMember->getStats().characterId) + ".png";
	Soft::UIGraphic* icon = new Soft::UIGraphic(iconPath.c_str());

	icon->position = { offset.x + 13, offset.y + 12 };

	healthBar = new Soft::UIRect();

	healthBar->setColor({ 255, 0, 0 });
	healthBar->setAlpha(0.5f);
	healthBar->setSize(PartyMemberHUD::HEALTH_BAR_REGION.width, PartyMemberHUD::HEALTH_BAR_REGION.height);

	healthBar->position = {
		offset.x + PartyMemberHUD::HEALTH_BAR_REGION.x,
		offset.y + PartyMemberHUD::HEALTH_BAR_REGION.y
	};

	magicBar = new Soft::UIRect();

	magicBar->setColor({ 0, 255, 0 });
	magicBar->setAlpha(0.5f);
	magicBar->setSize(PartyMemberHUD::MAGIC_BAR_REGION.width, PartyMemberHUD::MAGIC_BAR_REGION.height);

	magicBar->position = {
		offset.x + PartyMemberHUD::MAGIC_BAR_REGION.x,
		offset.y + PartyMemberHUD::MAGIC_BAR_REGION.y
	};

	add(slab);
	add(icon);
	add(healthBar);
	add(magicBar);
}

void PartyMemberHUD::onUpdate(int dt) {
	healthBar->clip(PartyMemberHUD::HEALTH_BAR_REGION.width * partyMember->getPercentageHP(), 5);
}

Soft::Region PartyMemberHUD::HEALTH_BAR_REGION = { 63, 19, 62, 5 };
Soft::Region PartyMemberHUD::MAGIC_BAR_REGION = { 63, 34, 62, 5 };

Soft::Coordinate PartyMemberHUD::positions[4] = {
	{ 395, 639 },
	{ 590, 639 },
	{ 786, 639 },
	{ 981, 639 }
};

/**
 * HUD
 * ---
 */
HUD::HUD(const Soft::Area& windowArea, const Party* party) {
	this->windowArea = windowArea;
	this->party = party;
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

	for (int i = 0; i < party->getSize(); i++) {
		const PartyMember* partyMember = party->getMemberByIndex(i);
		PartyMemberHUD* partyMemberHUD = new PartyMemberHUD(partyMember, i);

		add(partyMemberHUD);
	}
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
