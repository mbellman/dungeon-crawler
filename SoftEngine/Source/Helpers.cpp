#include <Helpers.h>
#include <random>
#include <math.h>
#include <string>

namespace Soft {

namespace RNG {
	namespace {
		std::default_random_engine engine;
		std::uniform_real_distribution<float> floatDistribution(0.0f, 1.0f);
	};

	float random() {
		return floatDistribution(engine);
	}

	float random(float low, float high) {
		return low + floor(random() * (high - low + 1));
	}

	std::string uuid() {
		return std::to_string(random()) + std::to_string(random());
	}
}

} // namespace Soft
