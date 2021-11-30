#pragma once
#include <string>
#include <map>
#include <vector>
#include "Graphics.h"
#include "Wallhack.h"
#include "Glow.h"
#include "Trigger.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_impl_dx9.h"
#include <mutex>

class Menu
{
	enum class OptionType {
		Checkbox,
		Slider,
		Label,
		ColorPicker3,
		ColorPicker4
	};

	struct Option {
		std::string label;
		OptionType type;
		void* address;
	};

	std::map<int, std::string> tabs;
	std::vector<std::vector<Option>> options;
	int activeTab;
	Graphics* gfx;
	HWND hwnd;
	bool run;

public:
	Menu(HWND hwnd, Graphics* gfx);
	void addWallhackOptions(Wallhack* wh);
	void addGlowOptions(Glow* glow);
	void addTriggerOptions(Trigger* trig);
	~Menu();

	void drawMenu();
};

