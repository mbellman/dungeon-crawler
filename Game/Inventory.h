#pragma once

#include <Items.h>
#include <vector>

class Inventory {
public:
	~Inventory();

	void addItem(const ItemData& itemData);
	const ItemData& getItemByIndex(int index) const;
	const std::vector<ItemData>& getItems() const;
	void removeItemByIndex(int index);

private:
	std::vector<ItemData> items;
};
