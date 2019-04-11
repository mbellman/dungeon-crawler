#include <Entities/Door.h>
#include <Level/LevelLoader.h>
#include <SoftEngine.h>
#include <MathUtils.h>
#include <GameUtils.h>

/**
 * Door
 * ----
 */
Door::Door(const DoorData& doorData, Soft::TextureBuffer* frameTexture, Soft::TextureBuffer* slabTexture) {
	this->doorData = doorData;
	this->frameTexture = frameTexture;
	this->slabTexture = slabTexture;
}

void Door::initialize() {
	Soft::ObjLoader frameLoader("./Assets/Models/door-frame.obj");
	Soft::ObjLoader slabLoader("./Assets/Models/door-slab.obj");

	Soft::Model* frame = new Soft::Model(frameLoader);

	frame->setTexture(frameTexture);
	frame->scale(GameUtils::HALF_TILE_SIZE);
	frame->position = GameUtils::getGridPositionVec3(doorData.position);

	slab = new Soft::Model(slabLoader);

	slab->setTexture(slabTexture);
	slab->setColor({ 180, 120, 50 });
	slab->scale(GameUtils::HALF_TILE_SIZE);
	slab->position = frame->position;

	if (doorData.axis == MathUtils::Axis::X) {
		frame->rotateDeg({ 0.0f, 90.0f, 0.0f });
		slab->rotateDeg({ 0.0f, 90.0f, 0.0f });
	}

	add(frame);
	add(slab);
}

const DoorData& Door::getDoorData() const {
	return doorData;
}

bool Door::isOpen() const {
	return hasOpened;
}

void Door::open() {
	Soft::Vec3 offset = { 0.0f, GameUtils::TILE_SIZE * 0.8f, 0.0f };

	slab->tweenTo(slab->position + offset, 1000, Soft::Ease::quadInOut);

	hasOpened = true;
}
