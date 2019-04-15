#pragma once

#include <random>
#include <string>
#include <math.h>
#include <SDL.h>

#define FAST_CLAMP(v, l, h) (v < l ? l : v > h ? h : v)
#define FAST_MAX(v1, v2) (v1 > v2 ? v1 : v2)
#define FAST_MIN(v1, v2) (v1 < v2 ? v1 : v2)
#define ARGB(a, r, g, b) (a << 24) | (r << 16) | (g << 8) | b

namespace Soft {

namespace Lerp {
	inline int lerp(int v1, int v2, float ratio) {
		return v1 + (int)(v2 - v1) * ratio;
	}

	inline float lerp(float v1, float v2, float ratio) {
		return v1 + (v2 - v1) * ratio;
	}
}

namespace RNG {
	float random();
	float random(float low, float high);
	std::string uuid();
}

namespace Math {
	inline float modf(float x, float y) {
		return x - y * floorf(x / y);
	}
}

} // namespace Soft
