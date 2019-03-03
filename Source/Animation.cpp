#include "Animation.h"

Animation::Animation()
{
	m_startTime = 0;
	m_endTime = 0;
}

Animation::Animation(const std::string & name, const glm::vec2 & times, const int & priority)
{
	m_name = name;
	m_startTime = times.x;
	m_endTime = times.y;
	m_priority = priority;
}
