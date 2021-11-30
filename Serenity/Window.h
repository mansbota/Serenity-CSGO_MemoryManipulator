#pragma once
#include <Windows.h>
#include <string_view>
#include "Keyboard.h"
#include "Mouse.h"
#include "Graphics.h"
#include <memory>

class Window
{
	static LRESULT WINAPI WndSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	static LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	LRESULT handleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;

	class WindowClass
	{
		std::wstring_view szWindowClass = L"Serenity";

	public:
		WindowClass();
		~WindowClass();
		std::wstring_view getClass() const noexcept { return szWindowClass; }
	};

	HWND m_hWnd;
	int m_width, m_height;
	static WindowClass m_wc;
	std::unique_ptr<Graphics> gfx;
	bool m_ImGuiOn = false, update = false;

public:
	Keyboard kbd;
	Mouse mouse;

	Window(std::wstring_view name, std::wstring_view gameWindowName);
	~Window();
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
	void setTitle(std::wstring_view title) const noexcept;
	Graphics* Gfx() { return gfx.get(); }
	static std::optional<int> processMessages(Window& window);
	HWND getHandle() const { return m_hWnd; }
	void toggleImGui() noexcept;
	bool isImGuiActive() const noexcept;
	bool isForeground() const noexcept;
	bool isGameForeground() const noexcept;
};

