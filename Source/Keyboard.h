#pragma once

#include <queue>
#include <bitset>

class Keyboard
{
public:
	friend class Window;
	class Event
	{
	public: 
		enum Type
		{
			Press,
			Release,
			Invalid
		};
	public:
		Event() : m_type(Invalid), m_code(0u) {}
		Event(Type type, unsigned char code) : m_type(type), m_code(code) {}

		bool isPressed() const { return m_type == Press; }
		bool isRelease() const { return m_type == Release; }
		bool isValid() const { return m_type != Invalid; }

		unsigned char getCode() const { return m_code; }

	private:
		Type m_type;
		unsigned char m_code;
	};

public:
	Keyboard() = default;
	
	bool keyIsPressed(unsigned char keyCode) const;
	bool keyIsEmpty() const;
	bool charIsEmpty() const;
	Event readKey();

	bool autoRepeatIsEnabled() const;
	void enableAutoRepeat();
	void disableAutoRepeat();

	void flush();
	void flushChar();
	void flushKey();

private:
	void onKeyPressed(unsigned char keyCode);
	void onKeyReleased(unsigned char keyCode);
	void onChar(unsigned char character);
	template<typename T>
	void trimBuffer(std::queue<T>& buffer);
private:
	static constexpr unsigned int nKeys = 256u;
	static constexpr unsigned int bufferSize = 4u;

	bool m_autoRepeatEnabled = false;
	std::bitset<nKeys> m_keyStates;
	std::queue<Event> m_keyBuffer;
	std::queue<char> m_charBuffer;
};

