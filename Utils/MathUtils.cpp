#include "MathUtils.h"

#include <Utils/Assert.h>

namespace Utils {
	float RandomFloat(const float min, const float max) {
		ASSERT(max > min);
		const float random = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
		// generate (in your case) a float between 0 and (4.5-.78)
		// then add .78, giving you a float between .78 and 4.5
		const float range = max - min;
		return (random * range) + min;
	}
}