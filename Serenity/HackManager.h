#pragma once
#include "MemWiz.h"
#include "Vectors.h"
#include "csgo.hpp"

class HackManager
{
	Mem::MemWiz* m_manager;
	uint32_t gameState{};
	uint32_t engineBase{};
	uint32_t maxPlayer{};
	std::uintptr_t radarBase{};
	
public:
	HackManager(Mem::MemWiz* manager);
	void updateState();
	int getGameState() const noexcept;
	uint32_t getMaxPlayers();
	std::string getName(int id);
};

//auto localPlayerSig = manager.sigScan(L"client_panorama.dll", "\x74\x2C\x8B\x0D\x00\x00\x00\x00\x8A", "xxxx????x", 4);
//
//auto localPlayerAddPtr = manager.read<std::uintptr_t>(localPlayerSig);