#include <Inventory.h>

/**
 * Inventory
 * ---------
 */
Inventory::~Inventory() {
	items.clear();
}

void Inventory::addItem(const ItemData& itemData) {
	items.push_back(itemData);
}

const ItemData& Inventory::getItemByIndex(int index) const {
	return items[index];
}

const std::vector<ItemData>& Inventory::getItems() const {
	return items;
}

void Inventory::removeItemByIndex(int index) {
	items.erase(items.begin() + index);
}
