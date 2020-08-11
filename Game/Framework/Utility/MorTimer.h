#pragma once

#include <chrono>

class MorTimer
{
public:
	MorTimer()
	{
		last = std::chrono::steady_clock::now();
	}
	float Mark()
	{
		const auto old = last;
		last = std::chrono::steady_clock::now();
		const std::chrono::duration<float> dur = last - old;
		return dur.count();
	}
	float Peek() const
	{
		return std::chrono::duration<float>( std::chrono::steady_clock::now() - last ).count();
	}
private:
	std::chrono::steady_clock::time_point last;
};