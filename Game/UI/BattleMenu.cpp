#include <UI/BattleMenu.h>
#include <UI/Frame.h>
#include <SDL_ttf.h>
#include <Party.h>
#include <Inventory.h>
#include <GameUtils.h>

/**
 * PartyMemberNameWindow
 * ---------------------
 */
PartyMemberNameWindow::PartyMemberNameWindow(TTF_Font* font, const PartyMember* partyMember, const Soft::Coordinate& position) {
	this->font = font;
	this->partyMember = partyMember;

	basePosition = position;
}

void PartyMemberNameWindow::initialize() {
	addFrame();
	addNameText();
	centerNameText();
}

void PartyMemberNameWindow::addFrame() {
	frame = new Frame({
		basePosition.x, basePosition.y,
		PartyMemberNameWindow::SIZE.width, PartyMemberNameWindow::SIZE.height
	}, 5);

	add(frame);
}

void PartyMemberNameWindow::addNameText() {
	nameText = new Soft::UIText();

	nameText->setFont(font);
	nameText->setValue(partyMember->getName());

	add(nameText);
}

/**
 * Centers the name text after its addition to the active scene
 * UI, upon which its width/height are properly determined.
 */
void PartyMemberNameWindow::centerNameText() {
	nameText->position = {
		basePosition.x + PartyMemberNameWindow::SIZE.width / 2 - nameText->getWidth() / 2,
		basePosition.y + PartyMemberNameWindow::SIZE.height / 2 - nameText->getHeight() / 2,
	};
}

void PartyMemberNameWindow::focus() {

}

void PartyMemberNameWindow::unfocus() {

}

Soft::Area PartyMemberNameWindow::SIZE = { 200, 50 };

/**
 * BattleMenu
 * ----------
 */
BattleMenu::BattleMenu(Party* party, Inventory* inventory) {
	menuFont = TTF_OpenFont("./Assets/Fonts/CourierPrime.ttf", 18);

	this->party = party;
	this->inventory = inventory;
}

BattleMenu::~BattleMenu() {
	TTF_CloseFont(menuFont);
}

void BattleMenu::initialize() {
	addBottomFrame();

	for (int i = 0; i < party->getSize(); i++) {
		const PartyMember* partyMember = party->getMemberByIndex(i);

		addPartyMemberNameWindow(partyMember, i);
	}
}

void BattleMenu::addBottomFrame() {
	Frame* frame = new Frame(BattleMenu::BOTTOM_FRAME_REGION, 8);

	add(frame);
}

void BattleMenu::addPartyMemberNameWindow(const PartyMember* partyMember, int index) {
	int outerWidth = PartyMemberNameWindow::SIZE.width + PartyMemberNameWindow::MARGIN;
	int groupWidth = party->getSize() * outerWidth;
	int firstWindowOffsetX = GameUtils::WINDOW_SIZE.width / 2 - groupWidth / 2 + PartyMemberNameWindow::MARGIN / 2;

	Soft::Coordinate position = {
		firstWindowOffsetX + outerWidth * index,
		500
	};

	PartyMemberNameWindow* window = new PartyMemberNameWindow(menuFont, partyMember, position);

	nameWindows.push_back(window);
	add(window);
}

Soft::Region BattleMenu::BOTTOM_FRAME_REGION = { 0, 570, 1200, 150 };
