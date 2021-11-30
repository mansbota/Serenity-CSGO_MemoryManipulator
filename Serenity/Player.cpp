#include "Player.h"

Player::Player(std::uintptr_t address, Mem::MemWiz* manager) noexcept 
	: m_manager{ manager }, address{ address }
{
	update();
}

void Player::update() noexcept
{
	playerData = m_manager->read<PlayerData>(address);	

	weapon.weaponHandle = getWeaponHandle();

	weapon.getWeaponInfo(m_manager);
}

Vec3 Player::getPos() const noexcept
{
	return *reinterpret_cast<Vec3*>(const_cast<char*>(playerData.data) + hazedumper::netvars::m_vecOrigin);
}

int32_t Player::getHealth() const noexcept
{
	return *reinterpret_cast<int32_t*>(const_cast<char*>(playerData.data) + hazedumper::netvars::m_iHealth);
}

int32_t Player::getTeam() const noexcept
{
	return *reinterpret_cast<int32_t*>(const_cast<char*>(playerData.data) + hazedumper::netvars::m_iTeamNum);
}

Vec2 Player::getAngles() const noexcept
{
	return *reinterpret_cast<Vec2*>(const_cast<char*>(playerData.data) + hazedumper::netvars::m_angEyeAnglesX);
}

std::string Player::getName() const noexcept
{
	return std::string();
}

unsigned int Player::getCrosshair() const noexcept
{
	return *reinterpret_cast<unsigned int*>(const_cast<char*>(playerData.data) + hazedumper::netvars::m_iCrosshairId);
}

bool Player::isDormant() const noexcept
{
	return *reinterpret_cast<bool*>(const_cast<char*>(playerData.data) + hazedumper::signatures::m_bDormant);
}

std::uintptr_t Player::getWeaponHandle() const noexcept
{
	return *reinterpret_cast<uintptr_t*>(const_cast<char*>(playerData.data) + hazedumper::netvars::m_hActiveWeapon);
}

bool Player::isDefusing() const noexcept
{
	return *reinterpret_cast<bool*>(const_cast<char*>(playerData.data) + hazedumper::netvars::m_bIsDefusing);
}

int Player::getFOV() const noexcept
{
	return *reinterpret_cast<int*>(const_cast<char*>(playerData.data) + hazedumper::netvars::m_iFOV);
}

std::uintptr_t Player::getBoneMatrix() const noexcept
{
	return *reinterpret_cast<std::uintptr_t*>(const_cast<char*>(playerData.data) + hazedumper::netvars::m_dwBoneMatrix);
}

std::string Player::getActiveWeapon() const noexcept
{
	if (isDefusing())
		return "Defusing...";

	// Get item definition index
	short index = m_manager->read<short>(weapon.weaponAddress + 0x2D80 + 0x40 + 0x1EA); //attributeManager + m_Item + m_iItemDefinitionIndex

	return getWeaponName(index);
}

int Player::getClipAmmo() const noexcept
{
	return weapon.ammoInfo.clip;
}

int Player::getReserveAmmo() const noexcept
{
	return weapon.ammoInfo.reserve;
}

Vec3 Player::getBonePos(int id) const noexcept
{
	uint32_t boneMatrix = getBoneMatrix();

	struct BoneStruct {
		float x;
		char pad1[12];
		float y;
		char pad2[12];
		float z;
	}bones;

	bones = m_manager->read<BoneStruct>(boneMatrix + 0x30 * id + 0x0C);

	Vec3 bonePos;
	bonePos.x = bones.x;
	bonePos.y = bones.y;
	bonePos.z = bones.z;

	return bonePos;
}

std::string Player::getWeaponName(short index) const noexcept
{
	switch (index)
	{
	case Weapon:: WEAPON_INVALID: return "None"; 
	case Weapon:: WEAPON_DEAGLE: return "Desert Eagle";
	case Weapon:: WEAPON_ELITE: return "Dual Berettas";
	case Weapon:: WEAPON_FIVESEVEN: return "Five-Seven";
	case Weapon:: WEAPON_GLOCK: return "Glock-18";
	case Weapon:: WEAPON_AK47: return "AK-47";
	case Weapon:: WEAPON_AUG: return "AUG";
	case Weapon:: WEAPON_AWP: return "AWP";  
	case Weapon:: WEAPON_FAMAS: return "FAMAS";  
	case Weapon:: WEAPON_G3SG1: return "G3SG1";  
	case Weapon:: WEAPON_GALILAR: return "Galil AR";  
	case Weapon:: WEAPON_M249: return "M249";  
	case Weapon:: WEAPON_M4A1: return "M4A1";  
	case Weapon:: WEAPON_MAC10: return "MAC-10";  
	case Weapon:: WEAPON_P90: return "P90";  
	case Weapon:: WEAPON_MP5: return "MP5-SD";  
	case Weapon:: WEAPON_UMP45: return "UMP-45";  
	case Weapon:: WEAPON_XM1014: return "XM1014";  
	case Weapon:: WEAPON_BIZON: return "PP-Bizon";  
	case Weapon:: WEAPON_MAG7: return "MAG-7";  
	case Weapon:: WEAPON_NEGEV: return "Negev";  
	case Weapon:: WEAPON_SAWEDOFF: return "Sawed-Off";  
	case Weapon:: WEAPON_TEC9: return "Tec-9";  
	case Weapon:: WEAPON_TASER: return "Zeus x27";  
	case Weapon:: WEAPON_HKP2000: return "P2000";  
	case Weapon:: WEAPON_MP7: return "MP7";  
	case Weapon:: WEAPON_MP9: return "MP9";  
	case Weapon:: WEAPON_NOVA: return "Nova";  
	case Weapon:: WEAPON_P250: return "P250";  
	case Weapon:: WEAPON_SCAR20: return "SCAR-20";  
	case Weapon:: WEAPON_SG556: return "SG 553";  
	case Weapon:: WEAPON_SSG08: return "SSG 08";  
	case Weapon:: WEAPON_KNIFEGG: return "Knife";  
	case Weapon:: WEAPON_KNIFE: return "Knife";  
	case Weapon:: WEAPON_FLASHBANG: return "Flashbang";  
	case Weapon:: WEAPON_HEGRENADE: return "High Explosive Grenade";  
	case Weapon:: WEAPON_SMOKEGRENADE: return "Smoke Grenade";  
	case Weapon:: WEAPON_MOLOTOV: return "Molotov";  
	case Weapon:: WEAPON_DECOY: return "Decoy Grenade";  
	case Weapon:: WEAPON_INCGRENADE: return "Incendiary Grenade";  
	case Weapon:: WEAPON_C4: return "C4 Explosive";  
	case Weapon:: WEAPON_HEALTHSHOT: return "Healthshot"; 
	case Weapon:: WEAPON_KNIFE_T: return "Knife"; 
	case Weapon:: WEAPON_M4A1_SILENCER: return "M4A1 Silenced"; 
	case Weapon:: WEAPON_USP_SILENCER: return "USP Silenced"; 
	case Weapon:: WEAPON_CZ75A: return "CZ75A"; 
	case Weapon:: WEAPON_REVOLVER: return "Revolver"; 
	case Weapon:: WEAPON_TAGRENADE: return "TA Granade"; 
	case Weapon:: WEAPON_FISTS: return "Fists"; 
	case Weapon:: WEAPON_BREACHCHARGE: return "Breach Granade"; 
	case Weapon:: WEAPON_TABLET: return "Tablet"; 
	case Weapon:: WEAPON_MELEE: return "Melee"; 
	case Weapon:: WEAPON_AXE: return "Axe"; 
	case Weapon:: WEAPON_HAMMER: return "Hammer"; 
	case Weapon:: WEAPON_SPANNER: return "Spanner"; 
	case Weapon:: WEAPON_KNIFE_GHOST: return "Ghost Knife"; 
	case Weapon:: WEAPON_FIREBOMB: return "Fire Bomb"; 
	case Weapon:: WEAPON_DIVERSION: return "Diversion"; 
	case Weapon:: WEAPON_FRAG_GRENADE: return "Frag Granade"; 
	case Weapon:: WEAPON_SNOWBALL: return "Snowball"; 
	case Weapon:: WEAPON_BAYONET: return "Bayonet"; 
	case Weapon:: WEAPON_KNIFE_FLIP: return "Flip Knife"; 
	case Weapon:: WEAPON_KNIFE_GUT: return "Gut Knife"; 
	case Weapon:: WEAPON_KNIFE_KARAMBIT: return "Karambit"; 
	case Weapon:: WEAPON_KNIFE_M9_BAYONET: return "M9 Bayonet Knife"; 
	case Weapon:: WEAPON_KNIFE_TACTICAL: return "Tactical Knife"; 
	case Weapon:: WEAPON_KNIFE_FALCHION: return "Falchion Knife"; 
	case Weapon:: WEAPON_KNIFE_SURVIVAL_BOWIE: return "Bowie Knife"; 
	case Weapon:: WEAPON_KNIFE_BUTTERFLY: return "Butterfly Knife"; 
	case Weapon:: WEAPON_KNIFE_PUSH: return "Push Knife"; 
	case Weapon:: WEAPON_KNIFE_URSUS: return "Ursus Knife"; 
	case Weapon:: WEAPON_KNIFE_GYPSY_JACKKNIFE: return "JackKnife"; 
	case Weapon:: WEAPON_KNIFE_STILETTO: return "Stiletto Knife"; 
	case Weapon:: WEAPON_KNIFE_WIDOWMAKER: return "Widowmaker Knife"; 
	case Weapon:: GLOVE_STUDDED_BLOODHOUND: return "Bloodhound Gloves"; 
	case Weapon:: GLOVE_T_SIDE: return "T Gloves"; 
	case Weapon:: GLOVE_CT_SIDE: return "CT Gloves"; 
	case Weapon:: GLOVE_SPORTY: return "Sporty Gloves"; 
	case Weapon:: GLOVE_SLICK: return "Slick Gloves"; 
	case Weapon:: GLOVE_LEATHER_WRAP: return "Leather Gloves"; 
	case Weapon:: GLOVE_MOTORCYCLE: return "Motorcycle Gloves";
	case Weapon:: GLOVE_SPECIALIST: return "Specialist Gloves";
	case Weapon:: GLOVE_HYDRA: return "Hydra Gloves";
	default: return "Unknown";
	}
}



