#pragma once

#include <cmath>
#include <System/Math.h>

namespace Soft {

struct Quaternion {
	float w;
	float x;
	float y;
	float z;

	static Quaternion fromAxisAngle(float angle, float x, float y, float z);
	RotationMatrix toRotationMatrix();
	Quaternion operator *(const Quaternion& q2) const;
};

} // namespace Soft
