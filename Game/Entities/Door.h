#pragma once

#include <SoftEngine.h>
#include <Entities/Interactible.h>
#include <Level/LevelLoader.h>
#include <MathUtils.h>

class Door : public Interactible<DoorData> {
public:
	Door(const DoorData& doorData, Soft::TextureBuffer* frameTexture, Soft::TextureBuffer* slabTexture);

	void initialize() override;
	bool isOpen() const;
	void open();

private:
	constexpr static int OPEN_ANIMATION_DURATION = 2000;

	Soft::Object* slab = nullptr;
	Soft::TextureBuffer* frameTexture = nullptr;
	Soft::TextureBuffer* slabTexture = nullptr;
	int openTime = 0;
};
