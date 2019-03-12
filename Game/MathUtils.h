#pragma once

#include <SDL.h>
#include <math.h>

namespace MathUtils {
	constexpr static float DEG_45 = M_PI / 4.0f;
	constexpr static float DEG_90 = DEG_45 * 2.0f;
	constexpr static float DEG_135 = DEG_45 * 3.0f;
	constexpr static float DEG_180 = DEG_45 * 4.0f;
	constexpr static float DEG_225 = DEG_45 * 5.0f;
	constexpr static float DEG_270 = DEG_45 * 6.0f;
	constexpr static float DEG_315 = DEG_45 * 7.0f;
	constexpr static float DEG_360 = DEG_45 * 8.0f;

	inline float modf(float x, float y) {
		return x - y * floorf(x / y);
	}
}
