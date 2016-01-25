#include "Hash.h"

#include "Assert.h"

namespace Utils {
	size_t Hash(const char* str) {
		ASSERT(str);
		size_t hashValue = 0;
		while (*str) {
			hashValue = hashValue * 101 + static_cast<size_t> (*str++);
		}
		return hashValue;
	}
}