#pragma once
#include <bitset>
#include <queue>
#include <optional>

class Keyboard
{
	friend class Window;

public:
	class Event
	{
	public:
		enum class Type
		{
			Press,
			Release
		};

		Event(Type type, unsigned char code) noexcept :
			m_type{ type }, m_code{ code } {}

		bool isPressed() const noexcept { return m_type == Type::Press; }
		bool isReleased() const noexcept { return m_type == Type::Release; }
		unsigned char getCode() const noexcept { return m_code; }

	private:
		Type m_type;
		unsigned char m_code;
	};

private:
	static constexpr unsigned char m_numKeys = 255u;
	static constexpr unsigned char m_bufferSize = 16u;
	std::bitset<m_numKeys> m_keystates;
	std::queue<Event> m_keybuffer;
	std::queue<char> m_charbuffer;

	void onKeyPressed(unsigned char keycode) noexcept;
	void onKeyReleased(unsigned char keycode) noexcept;
	void onChar(char character) noexcept;
	void clearState() noexcept { m_keystates.reset(); }
	template<typename T>
	static void trimBuffer(std::queue<T>& buffer) noexcept;

public:
	Keyboard() = default;
	Keyboard(const Keyboard&) = delete;
	Keyboard& operator=(const Keyboard&) = delete;
	
	std::optional<Event> readKey() noexcept;
	bool keyIsPressed(unsigned char keycode) const noexcept { return m_keystates[keycode]; }
	void flushKey() noexcept { m_charbuffer = std::queue<char>(); }

	std::optional<char> readChar() noexcept;
	void flushChar() noexcept { m_charbuffer = std::queue<char>(); }

	void flush() noexcept { flushKey(); flushChar(); }
};

