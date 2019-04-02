#pragma once

#include <SoftEngine.h>

struct SidedBlock : public Soft::Object {
	SidedBlock(int sidesMask, int subdivisions);

private:
	static Soft::Vec3 sideVertexPositions[6][4];
	static Soft::Vec2 sideUvs[4];

	void addSideVertices(Soft::Vec3 (&vertexPositions)[4], int subdivisions);
};
