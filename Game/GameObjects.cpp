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

	for (int i = 0; i < vertices.size(); i += 4) {
		addPolygon(i, i + 1, i + 2);
		addPolygon(i + 1, i + 3, i + 2);
	}
}

/**
 * Adds the vertices for a given block side, based on the provided
 * side corner vertex positions, using a given number of subdivisions.
 */
void SidedBlock::addSideVertices(Soft::Vec3 (&vertexPositions)[4], int subdivisions) {
	using namespace Soft;

	int s = subdivisions + 1;
	float ratioStep = 1.0f / s;

	for (int v = 0; v < s; v++) {
		float v_ratio = (float)v / s;

		for (int h = 0; h < s; h++) {
			float h_ratio = (float)h / s;

			for (int i = 0; i < 4; i++) {
				// For every vertex in the side face, we have to bilinearly interpolate
				// position and UV based on the number of subdivisions, and our current
				// horizontal/vertical progress along the face.
				float h_alpha = h_ratio + (i % 2 == 1 ? ratioStep : 0.0f);
				float v_alpha = v_ratio + (i > 1 ? ratioStep : 0.0f);

				addVertex(
					Vec3::lerp(
						Vec3::lerp(vertexPositions[0], vertexPositions[1], h_alpha),
						Vec3::lerp(vertexPositions[2], vertexPositions[3], h_alpha),
						v_alpha
					),
					Vec2::lerp(
						Vec2::lerp(SidedBlock::sideUvs[0], SidedBlock::sideUvs[1], h_alpha),
						Vec2::lerp(SidedBlock::sideUvs[2], SidedBlock::sideUvs[3], h_alpha),
						v_alpha
					)
				);
			}
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
