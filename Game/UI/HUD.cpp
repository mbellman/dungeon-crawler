#include <UI/HUD.h>
#include <GameUtils.h>
#include <Party.h>
#include <SoftEngine.h>
#include <SDL_ttf.h>
#include <string>

/**
 * PartyMemberHUD
 * --------------
 */
PartyMemberHUD::PartyMemberHUD(const PartyMember* partyMember, int index, TTF_Font* font) {
	this->partyMember = partyMember;
	this->index = index;
	this->font = font;
}

void PartyMemberHUD::initialize() {
	Soft::UIGraphic* slab = new Soft::UIGraphic("./Assets/UI/PartyHUD/slab.png");
	slab->position = getContainerOffset();

	add(slab);

	addIcon();
	addName();
	addBars();
}

void PartyMemberHUD::onUpdate(int dt) {
	healthBar->clip(healthBar->getWidth() * partyMember->getPercentageHP(), 5);
}

void PartyMemberHUD::addBars() {
	Soft::Coordinate offset = getContainerOffset();

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

	add(healthBar);
	add(magicBar);
}

void PartyMemberHUD::addIcon() {
	Soft::Coordinate offset = getContainerOffset();
	std::string iconPath = "./Assets/UI/PartyHUD/icon-" + std::to_string(partyMember->getStats().characterId) + ".png";
	Soft::UIGraphic* icon = new Soft::UIGraphic(iconPath.c_str());

	icon->position = { offset.x + 11, offset.y + 8 };

	add(icon);
}

void PartyMemberHUD::addName() {
	Soft::Coordinate offset = getContainerOffset();
	Soft::UIText* text = new Soft::UIText();
	text->setFont(font);
	text->setValue(partyMember->getName().c_str());
	text->position = { offset.x + 63, offset.y + 5 };

	add(text);
}

const Soft::Coordinate& PartyMemberHUD::getContainerOffset() {
	return PartyMemberHUD::positions[index];
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
HUD::HUD(const Party* party) {
	this->party = party;

	font = TTF_OpenFont("./Assets/Fonts/CourierPrime.ttf", 12);
}

void HUD::initialize() {
	using namespace GameUtils;

	int baseHeight = WINDOW_SIZE.height - (WINDOW_SIZE.height * RASTER_REGION.height / 100.0f);
	int baseY = WINDOW_SIZE.height - baseHeight;

	Soft::UIGraphic* leftColumn = new Soft::UIGraphic("./Assets/UI/column.png");
	leftColumn->position = { -30, 0 };

	Soft::UIGraphic* rightColumn = new Soft::UIGraphic("./Assets/UI/column.png");
	rightColumn->position = { WINDOW_SIZE.width - rightColumn->getWidth() + 30, 0 };

	Soft::UIGraphic* base = new Soft::UIGraphic("./Assets/UI/hud.png");
	base->position = { 0, baseY };

	lightBar = new Soft::UIRect();
	lightBar->setSize(136, 24);
	lightBar->setColor({ 0, 0, 255 });
	lightBar->setAlpha(0.5f);
	lightBar->position = { 200, baseY + 27 };

	add(leftColumn);
	add(rightColumn);
	add(base);
	add(lightBar);

	for (int i = 0; i < party->getSize(); i++) {
		const PartyMember* partyMember = party->getMemberByIndex(i);
		PartyMemberHUD* partyMemberHUD = new PartyMemberHUD(partyMember, i, font);

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
