#pragma once

#include <GameUtils.h>
#include <MathUtils.h>
#include <Items.h>
#include <Level/LevelLoader.h>
#include <Entities/Interactible.h>
#include <SoftEngine.h>

class Chest : public Interactible<ChestData> {
public:
	Chest(const ChestData& chestData, Soft::TextureBuffer* boxTexture, Soft::TextureBuffer* lidTexture);

	void initialize() override;
	const ItemData& getItemData() const;
	bool isOpen() const;
	void open(MathUtils::Direction direction);

private:
	Soft::TextureBuffer* boxTexture = nullptr;
	Soft::TextureBuffer* lidTexture = nullptr;
	Soft::Object* lid = nullptr;
	Soft::Light* itemLight = nullptr;
	bool hasOpened = false;
};
