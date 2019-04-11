#include <GameObjects.h>
#include <MathUtils.h>
#include <SoftEngine.h>

SidedBlock::SidedBlock(int sidesMask, int subdivisions) {
	using namespace MathUtils;

	if (sidesMask & Direction::UP) {
		addSideVertices(SidedBlock::sideVertexPositions[0], subdivisions);
	}

	if (sidesMask & Direction::DOWN) {
		addSideVertices(SidedBlock::sideVertexPositions[1], subdivisions);
	}

	if (sidesMask & Direction::LEFT) {
		addSideVertices(SidedBlock::sideVertexPositions[2], subdivisions);
	}

	if (sidesMask & Direction::RIGHT) {
		addSideVertices(SidedBlock::sideVertexPositions[3], subdivisions);
	}

	if (sidesMask & Direction::FORWARD) {
		addSideVertices(SidedBlock::sideVertexPositions[5], subdivisions);
	}

	if (sidesMask & Direction::BACKWARD) {
		addSideVertices(SidedBlock::sideVertexPositions[4], subdivisions);
	}

	int verticesPerLine = 2 + subdivisions;
	int verticesPerSide = verticesPerLine * verticesPerLine;

	for (int i = 0; i < totalSides; i++) {
		int vertexOffset = i * verticesPerSide;

		for (int v = 0; v < verticesPerLine - 1; v++) {
			for (int h = 0; h < verticesPerLine - 1; h++) {
				int n = vertexOffset + v * verticesPerLine + h;

				addPolygon(n, n + 1, n + verticesPerLine);
				addPolygon(n + 1, n + 1 + verticesPerLine, n + verticesPerLine);
			}
		}
	}
}

/**
 * Adds the vertices for a given block side, based on the provided
 * side corner vertex positions, using a given number of subdivisions.
 */
void SidedBlock::addSideVertices(Soft::Vec3 (&vertexPositions)[4], int subdivisions) {
	using namespace Soft;

	totalSides++;

	int verticesPerLine = 2 + subdivisions;
	float ratioStep = 1.0f / (verticesPerLine - 1);

	for (int v = 0; v < verticesPerLine; v++) {
		float v_ratio = v * ratioStep;

		for (int h = 0; h < verticesPerLine; h++) {
			float h_ratio = h * ratioStep;

			// For every vertex in the side face, we have to bilinearly interpolate
			// position and UV based on our current horizontal/vertical progress
			// along the face.
			addVertex(
				Vec3::lerp(
					Vec3::lerp(vertexPositions[0], vertexPositions[1], h_ratio),
					Vec3::lerp(vertexPositions[2], vertexPositions[3], h_ratio),
					v_ratio
				),
				Vec2::lerp(
					Vec2::lerp(SidedBlock::sideUvs[0], SidedBlock::sideUvs[1], h_ratio),
					Vec2::lerp(SidedBlock::sideUvs[2], SidedBlock::sideUvs[3], h_ratio),
					v_ratio
				)
			);
		}
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
