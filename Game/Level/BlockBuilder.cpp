#include <Level/BlockBuilder.h>
#include <SoftEngine.h>
#include <GameConstants.h>

Soft::Object* BlockBuilder::createBlockObject(int blockType) {
	using namespace GameConstants;

	switch (blockType) {
		case BlockTypes::GROUND: {
			return BlockBuilder::createSingleBlockFace();
		}
		case BlockTypes::CEILING: {
			Soft::Mesh* mesh = BlockBuilder::createSingleBlockFace();

			mesh->rotateDeg({ 180.0f, 0.0f, 0.0f });
			mesh->position = { 0.0f, -TILE_SIZE, TILE_SIZE };

			return mesh;
		}
		case BlockTypes::WALL_FRONT: {
			Soft::Mesh* mesh = BlockBuilder::createSingleBlockFace();

			mesh->rotateDeg({ -90.0f, 0.0f, 0.0f });
			mesh->position = { 0.0f, -TILE_SIZE, 0.0f };

			return mesh;
		}
		case BlockTypes::WALL_BACK: {
			Soft::Mesh* mesh = BlockBuilder::createSingleBlockFace();

			mesh->rotateDeg({ 90.0f, 0.0f, 0.0f });
			mesh->position = { 0.0f, 0.0f, TILE_SIZE };

			return mesh;
		}
		case BlockTypes::WALL_LEFT: {
			Soft::Mesh* mesh = BlockBuilder::createSingleBlockFace();

			mesh->rotateDeg({ 0.0f, 0.0f, -90.0f });
			mesh->position = { TILE_SIZE, 0.0f, 0.0f };

			return mesh;
		}
		case BlockTypes::WALL_RIGHT: {
			Soft::Mesh* mesh = BlockBuilder::createSingleBlockFace();

			mesh->rotateDeg({ 0.0f, 0.0f, 90.0f });
			mesh->position = { 0.0f, -TILE_SIZE, 0.0f };

			return mesh;
		}
		case BlockTypes::CUBE: {
			Soft::Cube* cube = new Soft::Cube(HALF_TILE_SIZE);

			cube->setFaceUVCoordinates(0.0f, 0.0f, 1.0f, 1.0f);
			cube->position = { HALF_TILE_SIZE, -HALF_TILE_SIZE, HALF_TILE_SIZE };

			return cube;
		}
		default: {
			return nullptr;
		}
	}
}

Soft::Mesh* BlockBuilder::createSingleBlockFace() {
	Soft::Mesh* mesh = new Soft::Mesh(1, 1, GameConstants::TILE_SIZE);

	mesh->setTextureInterval(1, 1);

	return mesh;
}
