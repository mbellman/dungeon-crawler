#include <GameObjects.h>
#include <MathUtils.h>
#include <SoftEngine.h>

SidedBlock::SidedBlock(int sidesMask) {
	using namespace MathUtils;

	if (sidesMask & Direction::UP) {
		addSideVertices(SidedBlock::sideVertexPositions[0]);
	}

	if (sidesMask & Direction::DOWN) {
		addSideVertices(SidedBlock::sideVertexPositions[1]);
	}

	if (sidesMask & Direction::LEFT) {
		addSideVertices(SidedBlock::sideVertexPositions[2]);
	}

	if (sidesMask & Direction::RIGHT) {
		addSideVertices(SidedBlock::sideVertexPositions[3]);
	}

	if (sidesMask & Direction::FORWARD) {
		addSideVertices(SidedBlock::sideVertexPositions[5]);
	}

	if (sidesMask & Direction::BACKWARD) {
		addSideVertices(SidedBlock::sideVertexPositions[4]);
	}

	for (int i = 0; i < vertices.size(); i += 4) {
		addPolygon(i, i + 1, i + 2);
		addPolygon(i + 1, i + 3, i + 2);
	}
}

void SidedBlock::addSideVertices(Soft::Vec3 (&vertexPositions)[4]) {
	for (int i = 0; i < 4; i++) {
		addVertex(vertexPositions[i], SidedBlock::sideUvs[i]);
	}
}

Soft::Vec3 SidedBlock::sideVertexPositions[6][4] = {
	// Top side
	{
		{ -1.0f, 1.0f, 1.0f },
		{ 1.0f, 1.0f, 1.0f },
		{ -1.0f, 1.0f, -1.0f },
		{ 1.0f, 1.0f, -1.0f }
	},
	// Bottom side
	{
		{ -1.0f, -1.0f, -1.0f },
		{ 1.0f, -1.0f, -1.0f },
		{ -1.0f, -1.0f, 1.0f },
		{ 1.0f, -1.0f, 1.0f }
	},
	// Left side
	{
		{ -1.0f, 1.0f, 1.0f },
		{ -1.0f, 1.0f, -1.0f },
		{ -1.0f, -1.0f, 1.0f },
		{ -1.0f, -1.0f, -1.0f }
	},
	// Right side
	{
		{ 1.0f, 1.0f, -1.0f },
		{ 1.0f, 1.0f, 1.0f },
		{ 1.0f, -1.0f, -1.0f },
		{ 1.0f, -1.0f, 1.0f }
	},
	// Front side
	{
		{ -1.0f, 1.0f, -1.0f },
		{ 1.0f, 1.0f, -1.0f },
		{ -1.0f, -1.0f, -1.0f },
		{ 1.0f, -1.0f, -1.0f }
	},
	// Back side
	{
		{ 1.0f, 1.0f, 1.0f },
		{ -1.0f, 1.0f, 1.0f },
		{ 1.0f, -1.0f, 1.0f },
		{ -1.0f, -1.0f, 1.0f }
	}
};

Soft::Vec2 SidedBlock::sideUvs[4] = {
	{ 0.0f, 0.0f },
	{ 1.0f, 0.0f },
	{ 0.0f, 1.0f },
	{ 1.0f, 1.0f }
};
