#include <UI/ItemMenu.h>
#include <UI/Frame.h>
#include <Inventory.h>
#include <SoftEngine.h>
#include <vector>

/**
 * ItemMenu
 * --------
 */
ItemMenu::ItemMenu(Inventory* inventory, TTF_Font* uiFont) {
	this->inventory = inventory;
	this->uiFont = uiFont;
}

ItemMenu::~ItemMenu() {
	itemTextSlots.clear();
}

void ItemMenu::initialize() {
	Frame* frame = new Frame({
		ItemMenu::OFFSET.x, ItemMenu::OFFSET.y,
		800, 400
	});

	add(frame);

	addItemTextSlots();
	populateItemTextSlots();
}

void ItemMenu::addItemTextSlots() {
	for (int i = 0; i < inventory->getItems().size(); i++) {
		Soft::UIText* text = new Soft::UIText();

		text->setFont(uiFont);

		text->position = {
			ItemMenu::OFFSET.x + ItemMenu::PADDING,
			ItemMenu::OFFSET.y + ItemMenu::PADDING + (i * 40)
		};

		itemTextSlots.push_back(text);
		add(text);
	}
}

void ItemMenu::cancel() {
	close();
}

void ItemMenu::close() {
	hasClosed = true;
}

void ItemMenu::confirm() {

}

bool ItemMenu::isClosed() const {
	return hasClosed;
}

void ItemMenu::populateItemTextSlots() {
	const std::vector<ItemData>& items = inventory->getItems();

	for (int i = slotOffset; i < items.size(); i++) {
		const ItemData& itemData = items[i];
		Soft::UIText* text = itemTextSlots[i];

		text->setValue(itemData.name);
	}
}

void ItemMenu::scrollDown() {

}

void ItemMenu::scrollUp() {

}

Soft::Coordinate ItemMenu::OFFSET = { 150, 100 };
