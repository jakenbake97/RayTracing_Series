#pragma once
#include <chrono>

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