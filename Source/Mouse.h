#pragma once

#include <memory>
#include <queue>

#include "Vendor/glm/glm.hpp"

class Mouse
{
public:
	friend class Application;
	class Event
	{
	public:
		enum Type
		{
			LPress,
			LRelease,
			RPress,
			RRelease,
			WheelUp,
			WheelDown,
			Move, 
			Invalid
		};
	public:
		Event() : m_type(Invalid), m_leftIsPressed(false), m_rightIsPressed(false),
			m_x(0), m_y(0){ }
		Event(Type type, const Mouse& parent) : m_type(type), 
			m_leftIsPressed(parent.m_leftIsPressed), m_rightIsPressed(false),
			m_x(0), m_y(0) {}

		bool isValid() const		{ return m_type != Invalid; }
		Type getType() const		{ return m_type; }
		glm::vec2 getPos() const	{ return glm::vec2(m_x, m_y); }
		int getPosX() const			{ return m_x; }
		int getPosY() const			{ return m_y; }
		bool leftIsPressed() const	{ return m_leftIsPressed; }
		bool rightIsPressed() const { return m_rightIsPressed; }
	private:
		Type m_type;
		bool m_leftIsPressed;
		bool m_rightIsPressed;
		int m_x;
		int m_y;
	};

public:
	Mouse() = default;
	glm::vec2 getPos() const { return glm::vec2(m_x, m_y); }
	int getPosX() const { return m_x; }
	int getPosY() const { return m_y; }
	bool leftIsPressed() const { return m_leftIsPressed; }
	bool rightIsPressed() const { return m_rightIsPressed; }
	bool isInWindow() const { return m_isInWindow; }
	Mouse::Event read();
	bool isEmpty() const { return m_buffer.empty(); }
	void flush() { m_buffer = std::queue<Event>(); }

private:
	void onMouseMove(int x, int y);
	void onMouseLeave();
	void onMouseEnter();
	void onLeftPressed(int x, int y);
	void onLeftReleased(int x, int y);
	void onRightPressed(int x, int y);
	void onRightReleased(int x, int y);
	void onWheelUp(int x, int y);
	void onWhelDown(int x, int y);
	void trimBuffer();

private:
	static constexpr unsigned int bufferSize = 4u;
	bool m_leftIsPressed;
	bool m_rightIsPressed;
	bool m_isInWindow = false;
	int m_x;
	int m_y;
	std::queue<Event> m_buffer;
};
