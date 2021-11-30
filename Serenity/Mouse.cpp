#include "Mouse.h"

void Mouse::onLButtonPressed() noexcept
{
	m_buffer.push(Event(Event::Type::LPress, * this));
	m_lButtonPressed = true;
	trimBuffer();
}

void Mouse::onRButtonPressed() noexcept
{
	m_buffer.push(Event(Event::Type::RPress, *this));
	m_rButtonPressed = true;
	trimBuffer();
}

void Mouse::onMButtonPressed() noexcept
{
	m_buffer.push(Event(Event::Type::MPress, *this));
	m_mButtonPressed = true;
	trimBuffer();
}

void Mouse::onRButtonReleased() noexcept
{
	m_buffer.push(Event(Event::Type::RRelease, *this));
	m_rButtonPressed = false;
	trimBuffer();
}

void Mouse::onLButtonReleased() noexcept
{
	m_buffer.push(Event(Event::Type::LRelease, *this));
	m_lButtonPressed = false;
	trimBuffer();
}

void Mouse::onMButtonReleased() noexcept
{
	m_buffer.push(Event(Event::Type::MRelease, *this));
	m_mButtonPressed = false;
	trimBuffer();
}

void Mouse::onMouseMove(int newx, int newy) noexcept
{
	m_x = newx;
	m_y = newy;
	m_buffer.push(Event(Event::Type::Move, *this));
	trimBuffer();
}

void Mouse::onWheelDown() noexcept
{
	m_buffer.push(Event(Event::Type::WheelDown, *this));
	trimBuffer();
}

void Mouse::onWheelUp() noexcept
{
	m_buffer.push(Event(Event::Type::WheelUp, *this));
	trimBuffer();
}

std::optional<Mouse::Event> Mouse::read() noexcept
{
	if (m_buffer.size() > 0u)
	{
		Event e = m_buffer.front();
		m_buffer.pop();
		return e;
	}

	return {};
}

void Mouse::trimBuffer() noexcept
{
	while (m_buffer.size() > m_bufferSize)
	{
		m_buffer.pop();
	}
}