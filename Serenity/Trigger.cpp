#include "Trigger.h"
#include "Player.h"
#include <memory>
#include <ctime>

#define KEY_DOWN -32768
#define KEY_UP 0

std::mt19937 Trigger::twister(static_cast<size_t>(std::time(nullptr)));

Trigger::Trigger(Mem::MemWiz* manager) :
	manager{ manager }, randomDelayMaxms{ 20 }, lastAimedAt{ 0 } {}

void Trigger::triggerbot()
{
	std::unique_ptr<Player> localPlayer{ new Player(manager->read<std::uintptr_t>(manager->mainModule() + hazedumper::signatures::dwLocalPlayer), manager) };

	unsigned int crosshairID = localPlayer->getCrosshair();

	if (crosshairID > 1 && crosshairID <= 64)
	{
		std::unique_ptr<Player> aimingAt(new Player(manager->read<std::uintptr_t>(manager->mainModule() + hazedumper::signatures::dwEntityList + (crosshairID - 1) * 0x10), manager));

		if (aimingAt->getTeam() != localPlayer->getTeam() && aimingAt->getHealth() > 0 && !aimingAt->isDormant())
		{
			short index = manager->read<short>(localPlayer->weapon.weaponAddress + 0x2D80 + 0x40 + 0x1EA); //attributeManager + m_Item + m_iItemDefinitionIndex

			if (
				index != Player::Weapon::WEAPON_KNIFEGG &&
				index != Player::Weapon::WEAPON_KNIFE &&
				index != Player::Weapon::WEAPON_FLASHBANG &&
				index != Player::Weapon::WEAPON_HEGRENADE &&
				index != Player::Weapon::WEAPON_SMOKEGRENADE &&
				index != Player::Weapon::WEAPON_MOLOTOV &&
				index != Player::Weapon::WEAPON_DECOY &&
				index != Player::Weapon::WEAPON_INCGRENADE &&
				index != Player::Weapon::WEAPON_C4 &&
				index != Player::Weapon::WEAPON_TAGRENADE &&
				index != Player::Weapon::WEAPON_FISTS &&
				index != Player::Weapon::WEAPON_BREACHCHARGE
				)
			{
				auto accuracyPen = manager->read<float>(localPlayer->weapon.weaponAddress + hazedumper::netvars::m_fAccuracyPenalty);

				if (accuracyPen < 0.09f)
				{
					float delay;

					switch (index) 
					{
					case Player::Weapon::WEAPON_AK47: case Player::Weapon::WEAPON_FAMAS: case Player::Weapon::WEAPON_GALILAR:
						delay = 3.1f;
						break;
					case Player::Weapon::WEAPON_CZ75A: case Player::Weapon::WEAPON_FIVESEVEN: case Player::Weapon::WEAPON_GLOCK: case Player::Weapon::WEAPON_P250: case Player::Weapon::WEAPON_HKP2000: case Player::Weapon::WEAPON_TEC9: case Player::Weapon::WEAPON_USP_SILENCER:
						delay = 5.0f;
						break;
					case Player::Weapon::WEAPON_DEAGLE:
						delay = 10.0f;
						break;
					case Player::Weapon::WEAPON_AWP: case Player::Weapon::WEAPON_SSG08:
						delay = 0.15f;
						break;
					case Player::Weapon::WEAPON_M4A1_SILENCER: case Player::Weapon::WEAPON_M4A1:
						delay = 1.5f;
						break;
					default:
						delay = .5f;
						break;
					}

					int sleepTime = static_cast<int>(distance(localPlayer->getPos(), aimingAt->getPos()) * 0.0254 * delay);
					
					if (lastAimedAt != crosshairID)
						Sleep(std::uniform_int_distribution{ 1, 10 }(twister));
					else
						Sleep(sleepTime + std::uniform_int_distribution{ 1, randomDelayMaxms }(twister));

					manager->write(manager->mainModule() + hazedumper::signatures::dwForceAttack, 5);

					Sleep(35);

					manager->write(manager->mainModule() + hazedumper::signatures::dwForceAttack, 4);

					aimingAt->update();

					if (aimingAt->getHealth() < 1)
					{
						Sleep(35);
						randomShoot(sleepTime);
					}
				}
			}
		}

		lastAimedAt = crosshairID;
	}
}

void Trigger::randomShoot(int sleepTime)
{
	for (int i = std::uniform_int_distribution{ 0 , 2 }(twister); i > 0; i--)
	{
		Sleep(sleepTime + std::uniform_int_distribution{ 1, randomDelayMaxms }(twister));

		manager->write(manager->mainModule() + hazedumper::signatures::dwForceAttack, 5);

		Sleep(35);

		manager->write(manager->mainModule() + hazedumper::signatures::dwForceAttack, 4);
	}
}

bool Trigger::triggerActive()
{
	if (GetAsyncKeyState(VK_LBUTTON) == KEY_DOWN)
		return false;

	if (GetAsyncKeyState(VK_LMENU) == KEY_DOWN)
		return true;

	return false;
}