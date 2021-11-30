#pragma once
#include "MemWiz.h"
#include "Player.h"
#include "csgo.hpp"
#include <mutex>

class Glow
{
	struct GlowStruct
	{
		float R;
		float G;
		float B;
		float m_flGlowAlpha;
		char m_unk[4];
		float m_flUnk;
		float m_flBloomAmount;
		float m_flUnk1;
		bool m_bRenderWhenOccluded;
		bool m_bRenderWhenUnoccluded;
		bool m_bFullBloomRender;
		char m_unk1;
		int m_nFullBloomStencilTestValue;
		int m_nGlowStyle;
		int m_nSplitScreenSlot;
		int m_nNextFreeSlot;
	}glow;

	struct GlowInfo {
		int glowArray;
		int glowCount;
	}glowInfo;

	Mem::MemWiz* m_manager;

	int getClassID(uintptr_t Entity);

public:
	float colorWeapons[4], colorAllies[4], colorEnemies[4];
	bool bGlow;

	Glow(Mem::MemWiz* manager);
	~Glow();
	void setGlow();
	void turnOff();
};

