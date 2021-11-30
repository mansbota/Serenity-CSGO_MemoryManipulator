#include "App.h"
#include "MemWiz.h"
#include "HackManager.h"
#include "Glow.h"
#include "Trigger.h"
#include "Wallhack.h"
#include "Player.h"
#include "csgo.hpp"
#include <thread>
#include "Menu.h"

App::App() : m_window(L"Beautiful Beast", L"Counter-Strike: Global Offensive") {}

void App::glowThread(std::atomic<bool>& flag, Mem::MemWiz manager, Menu* menu)
{
	Glow walls(&manager);

	menu->addGlowOptions(&walls);

	bool turnedOff = false;

	while(!flag)
	{
		if (!walls.bGlow && !turnedOff)
		{
			walls.turnOff();
			turnedOff = true;
		}

		if (walls.bGlow)
		{
			walls.setGlow();
			turnedOff = false;
		}

		Sleep(1);
	}
}

void App::triggerThread(std::atomic<bool>& flag, Mem::MemWiz manager, Menu* menu)
{
	Trigger triggerBot(&manager);

	menu->addTriggerOptions(&triggerBot);

	while (!flag)
	{
		if (triggerBot.triggerActive())
			triggerBot.triggerbot();

		Sleep(4);
	}
}

int App::run()
{
	Mem::MemWiz manager(L"csgo.exe", L"client_panorama.dll");

	HackManager csgo(&manager);

	Wallhack wh{ &manager, m_window.Gfx(), manager.mainModule() + hazedumper::signatures::dwViewMatrix, &csgo };

	Menu menu(m_window.getHandle(), m_window.Gfx());

	menu.addWallhackOptions(&wh);

	std::atomic<bool> flag = false;

	std::thread gThread(&App::glowThread, std::ref(flag), manager, &menu);

	std::thread tThread(&App::triggerThread, std::ref(flag), manager, &menu);

	while (manager.processActive())
	{
		csgo.updateState();

		if (GetAsyncKeyState(VK_INSERT) & 1)
			m_window.toggleImGui();

		m_window.Gfx()->startFrame();

		if (m_window.isGameForeground() && wh.bWallhack && csgo.getGameState() == 6)
			wh.wallhack();

		if (m_window.isImGuiActive())
			menu.drawMenu();

		m_window.Gfx()->endFrame();

		if (auto code = Window::processMessages(m_window))
		{
			flag = true;
			gThread.join();
			tThread.join();
			return *code;
		}

		Sleep(10);
	}

	flag = true;
	gThread.join();
	tThread.join();
	return -1;
}