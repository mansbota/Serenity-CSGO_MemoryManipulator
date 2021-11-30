#pragma once
#include "WinException.h"
#include "Graphics.h"
#include "Player.h"
#include "Vectors.h"
#include "HackManager.h"

class Wallhack 
{
	Mem::MemWiz* m_manager;
	HackManager* hm;
	Graphics* m_pGfx;
	std::uintptr_t matrixAdd;
	float boxColor[3], textColor[3];
	float scaling, heightRatio;

	struct ViewMatrix
	{
		float viewMatrix[16];
	};
	 
public:
	Wallhack(Mem::MemWiz* manager, Graphics* gfx, std::uintptr_t address, HackManager* hm);
	void wallhack() const;
	float* getBoxColor() noexcept;
	float* getTextColor() noexcept;
	bool bWallhack;

private:
	bool worldToScreen(Vec3 pos, Vec2& screenCords, float ViewMatrix[]) const noexcept;
	void draw(float x, float y, const Player& you, const Player& player, int i) const;
};
