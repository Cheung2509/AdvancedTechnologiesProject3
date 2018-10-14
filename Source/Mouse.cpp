#include "Mouse.h"

Mouse::Event Mouse::read()
{
	if (m_buffer.size() > 0u)
	{
		Mouse::Event e = m_buffer.front();
		m_buffer.pop();
		return e;
	}
	else
	{
		return Mouse::Event();
	}
}

void Mouse::onMouseMove(int x, int y)
{
	m_x = x;
	m_y = y;

	m_buffer.push(Mouse::Event(Mouse::Event::Move, *this));
	trimBuffer();
}

void Mouse::onMouseLeave()
{
	m_isInWindow = false;
}

void Mouse::onMouseEnter()
{
	m_isInWindow = true;
}

void Mouse::onLeftPressed(int x, int y)
{
	m_leftIsPressed = true;

	m_buffer.push(Mouse::Event(Mouse::Event::LPress, *this));
	trimBuffer();
}

void Mouse::onLeftReleased(int x, int y)
{
	m_leftIsPressed = false;

	m_buffer.push(Mouse::Event(Mouse::Event::LRelease, *this));
	trimBuffer();
}

void Mouse::onRightPressed(int x, int y)
{
	m_rightIsPressed = true;

	m_buffer.push(Mouse::Event(Mouse::Event::RPress, *this));
	trimBuffer();
}

void Mouse::onRightReleased(int x, int y)
{
	m_leftIsPressed = false;

	m_buffer.push(Mouse::Event(Mouse::Event::RRelease, *this));
	trimBuffer();
}

void Mouse::onWheelUp(int x, int y)
{
	m_buffer.push(Mouse::Event(Mouse::Event::WheelUp, *this));
	trimBuffer();
}

void Mouse::onWhelDown(int x, int y)
{
	m_buffer.push(Mouse::Event(Mouse::Event::WheelDown, *this));
	trimBuffer();
}

void Mouse::trimBuffer()
{
	while (m_buffer.size() > bufferSize)
	{
		m_buffer.pop();
	}
}
