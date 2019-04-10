#pragma once

#include <SoftEngine.h>
#include <Level/LevelLoader.h>
#include <MathUtils.h>

class Door : public Soft::Entity {
public:
	Door(const DoorData& doorData);

	void initialize() override;
	const DoorData& getDoorData() const;
	bool isOpen() const;
	void open();

private:
	DoorData doorData;
	Soft::Object* slab = nullptr;
	bool hasOpened = false;
};
