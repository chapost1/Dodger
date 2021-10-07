#pragma once
#include "pch.h"

namespace {
	int init() {
		srand((unsigned int)time(0));
		return 0;
	}
	int _ = init();
}

class LuckUtils {
public:
	static int RandomOutOf(const int& outOf);
	static int RandomInRange(const int& min, const int& max);
	static BOOL TossACoin();
	static BOOL RouletteSpin(const int& outOf);
	static BOOL FailRouletteSpin(const int& outOf);

	virtual ~LuckUtils() {}
};
