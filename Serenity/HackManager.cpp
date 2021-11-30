#include "HackManager.h"

HackManager::HackManager(Mem::MemWiz* manager) :
	m_manager{ manager }
{
	engineBase = m_manager->read<uint32_t>(m_manager->getModule(L"engine.dll") + hazedumper::signatures::dwClientState);

	radarBase = m_manager->read<uintptr_t>(m_manager->read<uintptr_t>(m_manager->mainModule() + hazedumper::signatures::dwRadarBase) + 0x74);
}

void HackManager::updateState()
{
	gameState = m_manager->read<uint32_t>(engineBase + static_cast<uintptr_t>(hazedumper::signatures::dwClientState_State));
}

int HackManager::getGameState() const noexcept
{
	return gameState;
}

uint32_t HackManager::getMaxPlayers()
{
	return m_manager->read<uint32_t>(engineBase + hazedumper::signatures::dwClientState_MaxPlayer);
}

std::string HackManager::getName(int i)
{
	struct NameStruct
	{
		char name[32];
	};

	return std::string(m_manager->read<NameStruct>(radarBase + 0x2E8 + (0x168 + 0x24) * i - 0x18 * (i - 1)).name);
}
	