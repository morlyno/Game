#pragma once

#include <chrono>

class MorTimer
{
public:
	MorTimer() noexcept
	{
		last = std::chrono::steady_clock::now();
	}
	float Mark() noexcept
	{
		const auto old = last;
		last = std::chrono::steady_clock::now();
		const std::chrono::duration<float> dur = last - old;
		return dur.count();
	}
	float Peek() const noexcept
	{
		return std::chrono::duration<float>( std::chrono::steady_clock::now() - last ).count();
	}
	float TimePassed() const noexcept
	{
		return std::chrono::duration<float>( std::chrono::steady_clock::now().time_since_epoch() ).count();
	}
private:
	std::chrono::steady_clock::time_point last;
};