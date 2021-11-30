#include "Menu.h"

Menu::Menu(HWND hwnd, Graphics* gfx) :
	gfx{ gfx }, activeTab{ 0 }, hwnd{ hwnd },
	tabs{ {0, "Visuals"}, {1, "Trigger"} }, run{ true }
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX9_Init(gfx->getDevice());

	ImGuiStyle& style = ImGui::GetStyle();

	style.WindowPadding = ImVec2(15, 15);
	style.WindowRounding = 5.0f;
	style.FramePadding = ImVec2(5, 5);
	style.FrameRounding = 4.0f;
	style.ItemSpacing = ImVec2(12, 8);
	style.ItemInnerSpacing = ImVec2(8, 6);
	style.IndentSpacing = 25.0f;
	style.ScrollbarSize = 15.0f;
	style.ScrollbarRounding = 9.0f;
	style.GrabMinSize = 5.0f;
	style.GrabRounding = 3.0f;

	style.Colors[ImGuiCol_Text] = ImVec4(0.80f, 0.80f, 0.83f, 1.00f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.05f, 0.07f, .5f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.80f, 0.80f, 0.83f, 0.88f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 0.98f, 0.95f, 0.75f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);
	style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(1.00f, 0.98f, 0.95f, 0.73f);

	ImGui::StyleColorsDark();

	options.resize(10);
}

void Menu::addWallhackOptions(Wallhack* wh)
{
	options[0].push_back(Option{ "Enable ESP", OptionType::Checkbox, (void*)&wh->bWallhack });
	options[0].push_back(Option{ "ESP Color", OptionType::ColorPicker3, (void*)wh->getBoxColor() });
	options[0].push_back(Option{ "Text Color", OptionType::ColorPicker3, (void*)wh->getTextColor() });
}

void Menu::addGlowOptions(Glow* glow)
{
	options[0].push_back(Option{ "Enable Glow", OptionType::Checkbox, (void*)&glow->bGlow });
	options[0].push_back(Option{ "Enemy Glow Color", OptionType::ColorPicker4, (void*)glow->colorEnemies });
	options[0].push_back(Option{ "Friendly Glow Color", OptionType::ColorPicker4, (void*)glow->colorAllies });
	options[0].push_back(Option{ "Weapon Glow Color", OptionType::ColorPicker4, (void*)glow->colorWeapons });
}

void Menu::addTriggerOptions(Trigger* trig)
{
	options[1].push_back(Option{ "Triggerbot accuracy (lower is better but more obvious)", OptionType::Label, nullptr });
	options[1].push_back(Option{ "Randomizer", OptionType::Slider, (void*)&trig->randomDelayMaxms });
}

Menu::~Menu()
{
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void Menu::drawMenu()
{
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("Serenity", &run);

	for (const auto& tab : tabs)
	{
		ImGui::SameLine();
		if (ImGui::Button(tab.second.c_str(), ImVec2(150, 25)))
			activeTab = tab.first;
	}

	for (const auto& option : options[activeTab])
	{
		switch (option.type)
		{
		case OptionType::Checkbox:
			ImGui::Checkbox(option.label.c_str(), (bool*)option.address);
			break;
		case OptionType::ColorPicker3:
			ImGui::ColorEdit3(option.label.c_str(), (float*)option.address);
			break;
		case OptionType::ColorPicker4:
			ImGui::ColorEdit4(option.label.c_str(), (float*)option.address);
			break;
		case OptionType::Slider:
			ImGui::SliderInt(option.label.c_str(), (int*)option.address, 2, 1000);
			break;
		case OptionType::Label:
			ImGui::Text(option.label.c_str());
			break;
		}
	}

	ImGui::End();

	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

	if (!run)
		SendMessage(hwnd, WM_CLOSE, NULL, NULL);
}
