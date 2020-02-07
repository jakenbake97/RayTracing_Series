#pragma once
#include <cstdlib>

inline double DRand()
{
	return rand() / (RAND_MAX + 1.0);
}
