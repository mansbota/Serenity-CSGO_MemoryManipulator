#include "Keyboard.h"

std::optional<Keyboard::Event> Keyboard::readKey() noexcept
{
	if (m_keybuffer.size() > 0u)
	{
		Keyboard::Event e = m_keybuffer.front();
		m_keybuffer.pop();
		return e;
	}
	return {};
}

std::optional<char> Keyboard::readChar() noexcept
{
	if (m_charbuffer.size() > 0u)
	{
		unsigned char charcode = m_charbuffer.front();
		m_charbuffer.pop();
		return charcode;
	}
	return {};
}

void Keyboard::onKeyPressed(unsigned char keycode) noexcept
{
	m_keystates[keycode] = true;
	m_keybuffer.push(Keyboard::Event(Keyboard::Event::Type::Press, keycode));
	trimBuffer(m_keybuffer);
}

void Keyboard::onKeyReleased(unsigned char keycode) noexcept
{
	m_keystates[keycode] = false;
	m_keybuffer.push(Keyboard::Event(Keyboard::Event::Type::Release, keycode));
	trimBuffer(m_keybuffer);
}

void Keyboard::onChar(char character) noexcept
{
	m_charbuffer.push(character);
	trimBuffer(m_charbuffer);
}

template<typename T>
void Keyboard::trimBuffer(std::queue<T>& buffer) noexcept
{
	while (buffer.size() > m_bufferSize)
	{
		buffer.pop();
	}
}

