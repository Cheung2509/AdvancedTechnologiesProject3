#pragma once

#include <chrono>

class Timer
{
public:
	Timer();
	~Timer();

	const float mark();

private:
	std::chrono::steady_clock::time_point m_last;
};