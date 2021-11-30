#pragma once
#include "MemWiz.h"
#include "Vectors.h"
#include "csgo.hpp"

class Player
{
	struct PlayerData
	{
		char data[50000];

	}playerData;

	Mem::MemWiz* m_manager;
	std::uintptr_t address;
	
public:
	Player() = default;
	Player(std::uintptr_t address, Mem::MemWiz* manager) noexcept;

	void update() noexcept;
	Vec3 getPos() const noexcept;
	int32_t getHealth() const noexcept;
	int32_t getTeam() const noexcept;
	Vec2 getAngles() const noexcept;
	std::string getName() const noexcept;
	unsigned int getCrosshair() const noexcept;
	bool isDormant() const noexcept;
	std::uintptr_t getWeaponHandle() const noexcept;
	bool isDefusing() const noexcept;
	int getFOV() const noexcept;
	std::uintptr_t getBoneMatrix() const noexcept;
	int getClipAmmo() const noexcept;
	int getReserveAmmo() const noexcept;
	
	std::string getActiveWeapon() const noexcept;
	std::string getWeaponName(short index) const noexcept;
	Vec3 getBonePos(int id) const noexcept;

	struct Weapon
	{
		friend class Player;

		std::uintptr_t weaponHandle;
		std::uintptr_t weaponAddress;

		struct AmmoInfo {
			int clip;
			char pad[4];
			int reserve;
		}ammoInfo;

		enum ItemDefinitionIndex
		{
			WEAPON_INVALID = -1,
			WEAPON_DEAGLE = 1,
			WEAPON_ELITE,
			WEAPON_FIVESEVEN,
			WEAPON_GLOCK,
			WEAPON_AK47 = 7,
			WEAPON_AUG,
			WEAPON_AWP,
			WEAPON_FAMAS,
			WEAPON_G3SG1,
			WEAPON_GALILAR = 13,
			WEAPON_M249,
			WEAPON_M4A1 = 16,
			WEAPON_MAC10,
			WEAPON_P90 = 19,
			WEAPON_MP5 = 23,
			WEAPON_UMP45,
			WEAPON_XM1014,
			WEAPON_BIZON,
			WEAPON_MAG7,
			WEAPON_NEGEV,
			WEAPON_SAWEDOFF,
			WEAPON_TEC9,
			WEAPON_TASER,
			WEAPON_HKP2000,
			WEAPON_MP7,
			WEAPON_MP9,
			WEAPON_NOVA,
			WEAPON_P250,
			WEAPON_SHIELD,
			WEAPON_SCAR20,
			WEAPON_SG556,
			WEAPON_SSG08,
			WEAPON_KNIFEGG,
			WEAPON_KNIFE,
			WEAPON_FLASHBANG,
			WEAPON_HEGRENADE,
			WEAPON_SMOKEGRENADE,
			WEAPON_MOLOTOV,
			WEAPON_DECOY,
			WEAPON_INCGRENADE,
			WEAPON_C4,
			WEAPON_HEALTHSHOT = 57,
			WEAPON_KNIFE_T = 59,
			WEAPON_M4A1_SILENCER,
			WEAPON_USP_SILENCER,
			WEAPON_CZ75A = 63,
			WEAPON_REVOLVER,
			WEAPON_TAGRENADE = 68,
			WEAPON_FISTS,
			WEAPON_BREACHCHARGE,
			WEAPON_TABLET = 72,
			WEAPON_MELEE = 74,
			WEAPON_AXE,
			WEAPON_HAMMER,
			WEAPON_SPANNER = 78,
			WEAPON_KNIFE_GHOST = 80,
			WEAPON_FIREBOMB,
			WEAPON_DIVERSION,
			WEAPON_FRAG_GRENADE,
			WEAPON_SNOWBALL,
			WEAPON_BUMPMINE,
			WEAPON_BAYONET = 500,
			WEAPON_KNIFE_FLIP = 505,
			WEAPON_KNIFE_GUT,
			WEAPON_KNIFE_KARAMBIT,
			WEAPON_KNIFE_M9_BAYONET,
			WEAPON_KNIFE_TACTICAL,
			WEAPON_KNIFE_FALCHION = 512,
			WEAPON_KNIFE_SURVIVAL_BOWIE = 514,
			WEAPON_KNIFE_BUTTERFLY,
			WEAPON_KNIFE_PUSH,
			WEAPON_KNIFE_URSUS = 519,
			WEAPON_KNIFE_GYPSY_JACKKNIFE,
			WEAPON_KNIFE_STILETTO = 522,
			WEAPON_KNIFE_WIDOWMAKER,
			GLOVE_STUDDED_BLOODHOUND = 5027,
			GLOVE_T_SIDE = 5028,
			GLOVE_CT_SIDE = 5029,
			GLOVE_SPORTY = 5030,
			GLOVE_SLICK = 5031,
			GLOVE_LEATHER_WRAP = 5032,
			GLOVE_MOTORCYCLE = 5033,
			GLOVE_SPECIALIST = 5034,
			GLOVE_HYDRA = 5035
		};

	private:
		void getWeaponInfo(Mem::MemWiz* m_manager) noexcept
		{
			// Read entity address from handle
			weaponAddress = m_manager->read<uintptr_t>(m_manager->mainModule() + hazedumper::signatures::dwEntityList + ((weaponHandle & 0xFFF) - 1) * 0x10);

			ammoInfo = m_manager->read<AmmoInfo>(weaponAddress + hazedumper::netvars::m_iClip1);
		}
	}weapon;
};

