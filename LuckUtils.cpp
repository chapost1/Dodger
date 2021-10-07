#include "pch.h"
#include "LuckUtils.h"

int LuckUtils::RandomOutOf(const int& outOf) {
	return rand() % outOf;
}

BOOL LuckUtils::TossACoin() {
	return LuckUtils::RandomOutOf(2);
}

BOOL LuckUtils::RouletteSpin(const int& outOf) {
	int chances = outOf;
	if (outOf < 0) {
		chances = 0;
	}
	return LuckUtils::RandomOutOf(chances) == 0;
}

BOOL LuckUtils::FailRouletteSpin(const int& outOf) {
	return !LuckUtils::RouletteSpin(outOf);
}

int LuckUtils::RandomInRange(const int& min, const int& max) {
	return LuckUtils::RandomOutOf(max - min + 1) + min;
}
