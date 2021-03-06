#pragma once

#include <limits.h>
#include <Graphics/Color.h>
#include <SDL.h>

namespace Soft {

constexpr static float TAU = M_PI * 2.0f;
constexpr static float PI_HALF = M_PI / 2.0f;
constexpr static float DEG_TO_RAD = M_PI / 180.0f;
constexpr static float RAD_TO_DEG = 180.0f / M_PI;

constexpr static int LOG2_TABLE[12][2] = {
	{ 0, 0 },
	{ 1, 0 },
	{ 2, 1 },
	{ 4, 2 },
	{ 8, 3 },
	{ 16, 4 },
	{ 32, 5 },
	{ 64, 6 },
	{ 128, 7 },
	{ 256, 8 },
	{ 512, 9 },
	{ 1024, 10 }
};

constexpr static float NEAR_PLANE_DISTANCE = 30.0f;
constexpr static float MIPMAP_DISTANCE_INTERVAL = 800.0f;
constexpr static float LOD_DISTANCE_THRESHOLD = 2500.0f;
constexpr static int SERIAL_ILLUMINATION_NONSTATIC_TRIANGLE_LIMIT = 2500;

constexpr static int MIN_COLOR_LERP_INTERVAL = 2;
constexpr static int MAX_COLOR_LERP_INTERVAL = 20;
constexpr static int MIN_COVER_TRIANGLE_SIZE = 150;
constexpr static int MIN_TEXTURE_SAMPLE_INTERVAL = 2;
constexpr static int MAX_TEXTURE_SAMPLE_INTERVAL = 3;
constexpr static int MAX_VISIBILITY = INT_MAX;
constexpr static float MAX_CAMERA_PITCH = 89.0f * DEG_TO_RAD;
constexpr static int MAX_RASTER_FILTER_ZONES = 50;

constexpr static int RASTER_FILTER_ZONE_RANGE = 250;
constexpr static int TRIANGLE_POOL_SIZE = 100000;
constexpr static int GLOBAL_SECTOR_ID = -1;

constexpr static Color COLOR_BLACK = { 0, 0, 0 };
constexpr static Color COLOR_TRANSPARENT = { 255, 0, 255 };

constexpr static int CLICK_TIME_LIMIT = 150;

} // namespace Soft
