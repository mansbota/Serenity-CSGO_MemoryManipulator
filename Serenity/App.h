#pragma once
#include "Window.h"
#include "HackManager.h"
#include "Player.h"
#include <atomic>
#include <mutex>
#include <memory>
#include "Menu.h"

class App
{
	Window m_window;

public:
	App();
	int run();
	static void glowThread(std::atomic<bool>& flag, Mem::MemWiz manager, Menu* menu);
	static void triggerThread(std::atomic<bool>& flag, Mem::MemWiz manager, Menu* menu);
};