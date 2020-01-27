#pragma once
#include <chrono>

// A simple way to time the duration it takes to render a single ppm frame
class Timer
{
private:
	std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
public:
	Timer()
	{
		startTime = std::chrono::high_resolution_clock::now();
	}

	float GetDuration() const
	{
		const std::chrono::duration<float> duration = std::chrono::high_resolution_clock::now() - startTime;
		return duration.count();
	}
};