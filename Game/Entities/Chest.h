#pragma once

#include <GameUtils.h>
#include <MathUtils.h>
#include <Level/LevelLoader.h>
#include <SoftEngine.h>

class Chest : public Soft::Entity {
public:
	Chest(const ChestData& chestData, Soft::TextureBuffer* boxTexture, Soft::TextureBuffer* lidTexture);

	void initialize() override;
	const ChestData& getChestData() const;
	bool isOpen() const;
	void open(MathUtils::Direction direction);

private:
	ChestData chestData;
	Soft::TextureBuffer* boxTexture = nullptr;
	Soft::TextureBuffer* lidTexture = nullptr;
	Soft::Object* lid = nullptr;
	bool hasOpened = false;
};
