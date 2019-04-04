#include <Entities/Chest.h>
#include <Level/LevelLoader.h>
#include <GameUtils.h>
#include <MathUtils.h>
#include <SoftEngine.h>

/**
 * Chest
 * -----
 */
Chest::Chest(const ChestData& chestData) {
	this->chestData = chestData;
}

void Chest::initialize() {
	Soft::ObjLoader baseLoader("./Assets/Models/chest-base.obj");
	Soft::ObjLoader lidLoader("./Assets/Models/chest-lid.obj");

	Soft::Model* base = new Soft::Model(baseLoader);
	lid = new Soft::Model(lidLoader);

	GameUtils::rotateToDirection(base, chestData.direction);
	base->setColor({ 150, 100, 25 });
	base->scale(GameUtils::HALF_TILE_SIZE);
	base->position = GameUtils::getGridPositionVec3(chestData.position);
	base->position.y -= GameUtils::HALF_TILE_SIZE * 0.72f;

	GameUtils::rotateToDirection(lid, chestData.direction);
	lid->setColor({ 150, 100, 25 });
	lid->scale(GameUtils::HALF_TILE_SIZE);
	lid->position = base->position;

	add(base);
	add(lid);
}

const ChestData& Chest::getChestData() const {
	return chestData;
}

bool Chest::isOpen() const {
	return hasOpened;
}

void Chest::open(MathUtils::Direction direction) {
	Soft::Vec3 lidOpenOffset = GameUtils::getDirectionVector(direction) * GameUtils::HALF_TILE_SIZE * 0.25f;

	lid->tweenTo(lid->position + lidOpenOffset, 750, Soft::Ease::quadInOut);

	hasOpened = true;
}
