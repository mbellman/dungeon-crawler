#include <Entities/Chest.h>
#include <Level/LevelLoader.h>
#include <GameUtils.h>
#include <MathUtils.h>
#include <Items.h>
#include <SoftEngine.h>

/**
 * Chest
 * -----
 */
Chest::Chest(const ChestData& chestData, Soft::TextureBuffer* boxTexture, Soft::TextureBuffer* lidTexture) {
	this->chestData = chestData;
	this->boxTexture = boxTexture;
	this->lidTexture = lidTexture;
}

void Chest::initialize() {
	Soft::ObjLoader boxLoader("./Assets/Models/chest-box.obj");
	Soft::ObjLoader lidLoader("./Assets/Models/chest-lid.obj");

	Soft::Model* box = new Soft::Model(boxLoader);
	lid = new Soft::Model(lidLoader);
	itemLight = new Soft::Light();

	GameUtils::rotateToDirection(box, chestData.direction);
	box->scale(GameUtils::HALF_TILE_SIZE);
	box->setTexture(boxTexture);
	box->position = GameUtils::getGridPositionVec3(chestData.position);
	box->position.y -= GameUtils::HALF_TILE_SIZE * 0.72f;

	GameUtils::rotateToDirection(lid, chestData.direction);
	lid->scale(GameUtils::HALF_TILE_SIZE);
	lid->setTexture(lidTexture);
	lid->position = box->position;

	itemLight->setColor({ 255, 255, 255 });
	itemLight->power = 0.0f;
	itemLight->range = 200.0f;
	itemLight->position = box->position;

	add(box);
	add(lid);
	add(itemLight);
}

const ChestData& Chest::getChestData() const {
	return chestData;
}

const ItemData& Chest::getItemData() const {
	return items[chestData.itemType];
}

bool Chest::isOpen() const {
	return hasOpened;
}

void Chest::open(MathUtils::Direction direction) {
	Soft::Vec3 lidOpenOffset = GameUtils::getDirectionVector(direction) * GameUtils::HALF_TILE_SIZE * 0.25f;

	lid->tweenTo(lid->position + lidOpenOffset, 750, Soft::Ease::quadInOut);
	itemLight->power.tweenTo(4.0f, 2000, Soft::Ease::inOutWave);

	hasOpened = true;
}