#include <Level/BlockBuilder.h>
#include <SoftEngine.h>
#include <GameConstants.h>

Soft::Object* BlockBuilder::createBlockObject(int blockType) {
	switch (blockType) {
		case BlockTypes::GROUND: {
			return BlockBuilder::createSingleTileMesh();
		}
		case BlockTypes::CEILING: {
			Soft::Mesh* mesh = BlockBuilder::createSingleTileMesh();

			mesh->rotateDeg({ 180.0f, 0.0f, 0.0f });
			mesh->position = { 0.0f, -GameConstants::TILE_SIZE, GameConstants::TILE_SIZE };

			return mesh;
		}
		case BlockTypes::WALL_FRONT: {
			Soft::Mesh* mesh = BlockBuilder::createSingleTileMesh();

			mesh->rotateDeg({ -90.0f, 0.0f, 0.0f });
			mesh->position = { 0.0f, -GameConstants::TILE_SIZE, 0.0f };

			return mesh;
		}
		case BlockTypes::WALL_BACK: {
			Soft::Mesh* mesh = BlockBuilder::createSingleTileMesh();

			mesh->rotateDeg({ 90.0f, 0.0f, 0.0f });
			mesh->position = { 0.0f, 0.0f, 0.0f };

			return mesh;
		}
		case BlockTypes::WALL_LEFT: {
			Soft::Mesh* mesh = BlockBuilder::createSingleTileMesh();

			mesh->rotateDeg({ 0.0f, 0.0f, -90.0f });
			mesh->position = { GameConstants::TILE_SIZE, 0.0f, 0.0f };

			return mesh;
		}
		case BlockTypes::WALL_RIGHT: {
			Soft::Mesh* mesh = BlockBuilder::createSingleTileMesh();

			mesh->rotateDeg({ 0.0f, 0.0f, 90.0f });
			mesh->position = { 0.0f, -GameConstants::TILE_SIZE, 0.0f };

			return mesh;
		}
		case BlockTypes::CUBE: {
			return new Soft::Cube(GameConstants::TILE_SIZE);
		}
		default: {
			return nullptr;
		}
	}
}

Soft::Mesh* BlockBuilder::createSingleTileMesh() {
	Soft::Mesh* mesh = new Soft::Mesh(1, 1, GameConstants::TILE_SIZE);

	mesh->setTextureInterval(1, 1);

	return mesh;
}
