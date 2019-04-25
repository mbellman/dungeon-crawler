#pragma once

#include <SoftEngine.h>
#include <SDL_ttf.h>
#include <UI/Frame.h>
#include <Party.h>
#include <Inventory.h>
#include <vector>

class PartyMemberNameWindow : public Soft::Entity {
public:
	static Soft::Area SIZE;
	constexpr static int MARGIN = 100;

	PartyMemberNameWindow(TTF_Font* font, const PartyMember* partyMember, const Soft::Coordinate& position);

	void initialize() override;
	void focus();
	void unfocus();

private:
	Soft::Coordinate basePosition;
	TTF_Font* font = nullptr;
	const PartyMember* partyMember = nullptr;
	Soft::UIText* nameText = nullptr;
	Frame* frame = nullptr;

	void addFrame();
	void addNameText();
	void centerNameText();
};

class BattleMenu : public Soft::Entity {
public:
	BattleMenu(Party* party, Inventory* inventory);
	~BattleMenu();

	void initialize() override;

private:
	static Soft::Region BOTTOM_FRAME_REGION;

	TTF_Font* menuFont = nullptr;
	Party* party = nullptr;
	Inventory* inventory = nullptr;
	std::vector<PartyMemberNameWindow*> nameWindows;

	void addBottomFrame();
	void addPartyMemberNameWindow(const PartyMember* partyMember, int index);
};
