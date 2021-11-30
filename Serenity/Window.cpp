#include <Windows.h>
#include "Window.h"
#include "WinException.h"
#include "resource.h"
#include <dwmapi.h>

#pragma comment(lib,"dwmapi.lib")

Window::WindowClass Window::m_wc;

Window::WindowClass::WindowClass()
{
	WNDCLASSEX wc = {};
	wc.cbSize = sizeof(wc);
	wc.hInstance = GetModuleHandle(nullptr);
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = nullptr;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = static_cast<HICON>(LoadImage(GetModuleHandle(nullptr), MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 64, 64, 0));
	wc.hIconSm = nullptr;
	wc.style = 0u;
	wc.lpfnWndProc = WndSetup;
	wc.lpszClassName = szWindowClass.data();
	wc.lpszMenuName = nullptr;

	RegisterClassEx(&wc);
}

Window::WindowClass::~WindowClass()
{
	UnregisterClass(szWindowClass.data(), GetModuleHandle(nullptr));
}

Window::Window(std::wstring_view name, std::wstring_view gameWindowName) :
	m_width{ GetSystemMetrics(SM_CXSCREEN) }, m_height{ GetSystemMetrics(SM_CYSCREEN) }
{
	m_hWnd = CreateWindowEx(
		NULL,
		m_wc.getClass().data(),
		name.data(),
		WS_POPUP, 0, 0,
		m_width, m_height, nullptr, nullptr,
		GetModuleHandle(nullptr), this);

	if (m_hWnd == nullptr)
		throw WINLASTEXCEPT;

	gfx = std::make_unique<Graphics>(m_hWnd, m_width, m_height, gameWindowName);

	SetWindowLongPtr(m_hWnd, GWL_STYLE, WS_VISIBLE);
	SetWindowLongPtr(m_hWnd, GWL_EXSTYLE, WS_EX_TRANSPARENT | WS_EX_LAYERED | WS_EX_TOOLWINDOW);
	SetWindowPos(m_hWnd, HWND_NOTOPMOST, 0, 0, m_width, m_height, SWP_SHOWWINDOW);

	MARGINS margin{ -1 };
	DwmExtendFrameIntoClientArea(m_hWnd, &margin);
}

Window::~Window()
{
	DestroyWindow(m_hWnd);
}

void Window::setTitle(std::wstring_view title) const noexcept
{
	SetWindowText(m_hWnd, title.data());
}

std::optional<int> Window::processMessages(Window& window)
{
	MSG msg;

	HWND hwnd2 = GetForegroundWindow();
	HWND hwnd3 = GetWindow(hwnd2, GW_HWNDPREV);
	SetWindowPos(window.getHandle(), hwnd3, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
			return msg.wParam;

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return {};
}

void Window::toggleImGui() noexcept
{
	m_ImGuiOn = !m_ImGuiOn;

	long style = GetWindowLongPtr(m_hWnd, GWL_EXSTYLE);

	if (m_ImGuiOn)
	{
		style &= ~WS_EX_LAYERED;
		SetWindowLongPtr(m_hWnd,
			GWL_EXSTYLE, style);
	}
	else
	{
		style |= WS_EX_LAYERED;
		SetWindowLongPtr(m_hWnd,
			GWL_EXSTYLE, style);
	}

	update = true;
}

bool Window::isImGuiActive() const noexcept
{
	return m_ImGuiOn;
}

bool Window::isForeground() const noexcept
{
	return m_hWnd == GetForegroundWindow();
}

bool Window::isGameForeground() const noexcept
{
	return gfx.get()->getGameWindow() == GetForegroundWindow();
}

LRESULT WINAPI Window::WndSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	if (msg == WM_NCCREATE)
	{
		const auto structPtr = reinterpret_cast<CREATESTRUCTW*>(lParam);

		Window* wnd = reinterpret_cast<Window*>(structPtr->lpCreateParams);

		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(wnd));

		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(Window::WndProc));

		return wnd->handleMsg(hWnd, msg, wParam, lParam);
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT WINAPI Window::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	auto ptrWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

	return ptrWnd->handleMsg(hWnd, msg, wParam, lParam);
}

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT Window::handleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		return true;

	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
	case WM_SYSKEYDOWN: case WM_KEYDOWN:
		kbd.onKeyPressed(static_cast<unsigned char>(wParam));
		break;
	case WM_SYSKEYUP: case WM_KEYUP:
		kbd.onKeyReleased(static_cast<unsigned char>(wParam));
		break;
	case WM_CHAR:
		kbd.onChar(static_cast<unsigned char>(wParam));
		break;
	case WM_KILLFOCUS:
		kbd.clearState();
		break;
	case WM_MOUSEMOVE:
	{
		if (update && m_ImGuiOn)
		{
			INPUT in{};
			in.type = INPUT_MOUSE;
			in.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN | MOUSEEVENTF_RIGHTUP;

			SendInput(1, &in, sizeof(INPUT));
			update = false;
		}

		const POINTS pt = MAKEPOINTS(lParam);
		mouse.onMouseMove(pt.x, pt.y);
		break;
	}
	case WM_LBUTTONDOWN:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.onLButtonPressed();
		break;
	}
	case WM_LBUTTONUP:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.onLButtonReleased();
		break;
	}
	case WM_RBUTTONDOWN:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.onRButtonPressed();
		break;
	}
	case WM_RBUTTONUP:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.onRButtonReleased();
		break;
	}
	case WM_MBUTTONDOWN:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.onMButtonPressed();
		break;
	}
	case WM_MBUTTONUP:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.onMButtonReleased();
		break;
	}
	case WM_MOUSEWHEEL:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		if (GET_WHEEL_DELTA_WPARAM(wParam) > 0)
			mouse.onWheelUp();
		else
			mouse.onWheelDown();
		break;
	}
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}
