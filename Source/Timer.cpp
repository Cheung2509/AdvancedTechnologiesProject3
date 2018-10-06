#include "Timer.h"

Timer::Timer()
{
	m_last = std::chrono::steady_clock::now();
}

Timer::~Timer()
{
}

const float Timer::mark()
{
	auto old = m_last;
	m_last = std::chrono::steady_clock::now();
	const std::chrono::duration<float> frameTime = m_last - old;
	return frameTime.count();
}
