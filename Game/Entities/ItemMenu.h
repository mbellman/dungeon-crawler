#pragma once

#include <SoftEngine.h>
#include <Inventory.h>
#include <vector>

class ItemMenu : public Soft::Entity {
public:
	ItemMenu(Inventory* inventory, TTF_Font* uiFont);
	~ItemMenu();

	void initialize() override;
	void cancel();
	void confirm();
	bool isClosed() const;
	void scrollDown();
	void scrollUp();

private:
	static Soft::Coordinate OFFSET;
	constexpr static int PADDING = 30;

	Inventory* inventory = nullptr;
	TTF_Font* uiFont = nullptr;
	std::vector<Soft::UIText*> itemTextSlots;
	int slotOffset = 0;
	bool hasClosed = false;

	void addItemTextSlots();
	void close();
	void populateItemTextSlots();
};
