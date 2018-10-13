#include "Keyboard.h"

std::shared_ptr<Keyboard> Keyboard::instance = nullptr;

bool Keyboard::keyIsPressed(unsigned char keyCode) const
{
	return m_keyStates[keyCode];
}

bool Keyboard::keyIsEmpty() const
{
	return m_keyBuffer.empty();
}

bool Keyboard::charIsEmpty() const
{
	return m_charBuffer.empty();
}

Keyboard::Event Keyboard::readKey()
{
	if (m_keyBuffer.size() > 0u)
	{
		Keyboard::Event e = m_keyBuffer.front();
		m_keyBuffer.pop();
		return e;
	}
	else
	{
		return Keyboard::Event();
	}
}

bool Keyboard::autoRepeatIsEnabled() const
{
	return m_autoRepeatEnabled;
}

void Keyboard::enableAutoRepeat()
{
	m_autoRepeatEnabled = true;
}

void Keyboard::disableAutoRepeat()
{
	m_autoRepeatEnabled = false;
}

void Keyboard::flush()
{
	flushKey();
	flushChar();
}

void Keyboard::flushChar()
{
	m_charBuffer = std::queue<char>();
}

void Keyboard::flushKey()
{
	m_keyBuffer = std::queue<Event>();
}

std::shared_ptr<Keyboard> Keyboard::getInstance()
{
	if (!instance)
	{
		instance = std::make_shared<Keyboard>();
	}

	return instance;
}

void Keyboard::onKeyPressed(unsigned char keyCode)
{
	m_keyStates[keyCode] = true;
	m_keyBuffer.push(Keyboard::Event(Keyboard::Event::Press, keyCode));
	trimBuffer(m_keyBuffer);
}

void Keyboard::onKeyReleased(unsigned char keyCode)
{
	m_keyStates[keyCode] = false;
	m_keyBuffer.push(Keyboard::Event(Keyboard::Event::Press, keyCode));
	trimBuffer(m_keyBuffer);
}

void Keyboard::onChar(unsigned char character)
{
	m_charBuffer.push(character);
	trimBuffer(m_charBuffer);
}

template<typename T>
void Keyboard::trimBuffer(std::queue<T>& buffer)
{
	while (buffer.size() > bufferSize)
	{
		buffer.pop();
	}
}
