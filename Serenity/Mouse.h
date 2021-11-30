#pragma once
#include <tuple>
#include <queue>
#include <optional>

class Mouse
{
	friend class Window;

public:
	class Event
	{
	public:
		enum class Type
		{
			LPress,
			LRelease,
			RPress,
			RRelease,
			MPress,
			MRelease,
			Move,
			WheelUp,
			WheelDown
		};

		Event(Type type, const Mouse& parent) :
			m_type{ type },
			m_rButtonPressed{ parent.m_rButtonPressed },
			m_lButtonPressed{ parent.m_lButtonPressed },
			m_mButtonPressed{ parent.m_mButtonPressed },
			m_x{ parent.m_x },
			m_y{ parent.m_y } {}

		Type getType() const noexcept { return m_type; }
		std::pair<int, int> getPos() const noexcept { return { m_x, m_y }; }
		bool isRButtonPressed() const noexcept { return m_rButtonPressed; }
		bool isLButtonPressed() const noexcept { return m_lButtonPressed; }
		bool isMButtonPressed() const noexcept { return m_mButtonPressed; }

	private:
		Type m_type;
		int m_x, m_y;
		bool m_rButtonPressed;
		bool m_lButtonPressed;
		bool m_mButtonPressed;
	};

private:
	static constexpr unsigned char m_bufferSize = 16u;
	bool m_lButtonPressed = false;
	bool m_rButtonPressed = false;
	bool m_mButtonPressed = false;
	int m_x = 0, m_y = 0;
	std::queue<Event> m_buffer{};

	Mouse() = default;
	Mouse(const Mouse&) = delete;
	Mouse& operator=(const Mouse&) = delete;

	void flush() noexcept { m_buffer = std::queue<Event>(); }
	void trimBuffer() noexcept;

	void onLButtonPressed() noexcept;
	void onRButtonPressed() noexcept;
	void onMButtonPressed() noexcept;
	void onLButtonReleased() noexcept;
	void onRButtonReleased() noexcept;
	void onMButtonReleased() noexcept;
	void onMouseMove(int x, int y) noexcept;
	void onWheelUp() noexcept;
	void onWheelDown() noexcept;
	
public:
	bool isLButtonPressed() const noexcept { return m_lButtonPressed; }
	bool isRButtonPressed() const noexcept { return m_rButtonPressed; }
	bool isMButtonPressed() const noexcept { return m_mButtonPressed; }
	std::pair<int, int> getPos() const noexcept { return { m_x, m_y }; }
	std::optional<Event> read() noexcept;
};

