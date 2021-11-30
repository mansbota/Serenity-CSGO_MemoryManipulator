#include "Wallhack.h"
#include <memory>
#include "csgo.hpp"
#include <sstream>

Wallhack::Wallhack(Mem::MemWiz* manager, Graphics* gfx, std::uintptr_t address, HackManager* hm) : 
	m_manager(manager),
	m_pGfx(gfx), matrixAdd{ address },
	boxColor{ .8f, .8f, .8f },
	textColor{ .9f, .9f, .9f },
	bWallhack{ true },
	scaling{ 17 },
	heightRatio{ 3.25f },
	hm{ hm }
{}

void Wallhack::wallhack() const
{
	Vec2 screenCords;

	std::unique_ptr<Player>localPlayer{
			new Player(m_manager->read<std::uintptr_t>(m_manager->mainModule() + hazedumper::signatures::dwLocalPlayer), m_manager) };

	for (int i = 1; i <= 64; i++)
	{
		std::unique_ptr<Player> otherPlayer{ new Player(m_manager->read<uintptr_t>(m_manager->mainModule() + hazedumper::signatures::dwEntityList + (i * 0x10)), m_manager) };

		if (localPlayer->getTeam() == otherPlayer->getTeam())
			continue;

		if (otherPlayer->getTeam() != 2 && otherPlayer->getTeam() != 3)
			continue;

		if (otherPlayer->getHealth() < 1)
			continue;

		if (otherPlayer->getPos().x == 0.0f && otherPlayer->getPos().y == 0.0f && otherPlayer->getPos().z == 0.0f)
			continue;

		if (otherPlayer->isDormant())
			continue;

		if (worldToScreen(otherPlayer->getPos(), screenCords, m_manager->read<ViewMatrix>(matrixAdd).viewMatrix))
			draw(screenCords.x, screenCords.y, *localPlayer, *otherPlayer, i);
	}
}

bool Wallhack::worldToScreen(Vec3 pos, Vec2& screenCords, float matrix[]) const noexcept
{
	screenCords.x = pos.x * matrix[0] + pos.y * matrix[1] + pos.z * matrix[2] + matrix[3];
	screenCords.y = pos.x * matrix[4] + pos.y * matrix[5] + pos.z * matrix[6] + matrix[7];
	float w = pos.x * matrix[12] + pos.y * matrix[13] + pos.z * matrix[14] + matrix[15];

	if (w < .1f)
		return false;

	screenCords.x /= w;
	screenCords.y /= w;

	screenCords.x = (m_pGfx->getWidth() / 2.f * screenCords.x) + (screenCords.x + m_pGfx->getWidth() / 2.f);
	screenCords.y = -(m_pGfx->getHeight() / 2.f * screenCords.y) + (screenCords.y + m_pGfx->getHeight() / 2.f);

	return true;
}

void Wallhack::draw(float x, float y, const Player& you, const Player& player, int i) const
{
	float dist = distance(player.getPos(), you.getPos()), width, height;

	int mult;

	switch (you.getFOV())
	{
	case 40:
		width  =	(m_pGfx->getWidth() * scaling * 2.06f) / dist;
		height =	(m_pGfx->getHeight() * scaling * 2.46f * heightRatio) / dist;
		mult   =	(100 - player.getHealth()) / 4;
		break;
	case 45:
		width  =	(m_pGfx->getWidth() * scaling * 1.76f) / dist;
		height =	(m_pGfx->getHeight() * scaling * 2.16f * heightRatio) / dist;
		mult   =	(100 - player.getHealth()) / 5;
		break;
	case 15:
		width  =	(m_pGfx->getWidth() * scaling * 4.9f) / dist;
		height =	(m_pGfx->getHeight() * scaling * 6.6f * heightRatio) / dist;
		mult   =	(100 - player.getHealth()) / 2;
		break;
	case 10:
		width  =	(m_pGfx->getWidth() * scaling * 9.4f) / dist;
		height =	(m_pGfx->getHeight() * scaling * 10.f * heightRatio) / dist;
		mult  =		(100 - player.getHealth());
		break;
	default:
		width  =	(m_pGfx->getWidth() * scaling) / dist;
		height =	(m_pGfx->getHeight() * scaling * heightRatio) / dist;
		mult   =	(100 - player.getHealth()) / 10;
		break;
	}

	y -= height;
	x -= width / 2;

	m_pGfx->drawBorderBox(
		x,
		y, width, height,
		1.f,
		D3DCOLOR_XRGB(
		(int)(boxColor[0] * 255), 
		(int)(boxColor[1] * 255), 
		(int)(boxColor[2] * 255)));

	m_pGfx->drawFilledRect(
		x - 6,
		y + 135 * mult / (dist / (m_pGfx->getHeight() / 28)),
		2,
		height - 135 * mult / (dist / (m_pGfx->getHeight() / 28)),
		D3DCOLOR_XRGB(
			200 - player.getHealth() * 2,
			50 + player.getHealth() * 2,
			0));

	std::string activeWeapon = player.getActiveWeapon();

	m_pGfx->drawString(
		hm->getName(i),
		m_pGfx->centerText(x, width, static_cast<float>(activeWeapon.size())),
		y - 15,
		D3DCOLOR_XRGB(
		(int)(textColor[0] * 255),
		(int)(textColor[1] * 255),
		(int)(textColor[2] * 255)));

	m_pGfx->drawString(
		activeWeapon,
		m_pGfx->centerText(x, width, static_cast<float>(activeWeapon.size())),
		y + height + 4,
		D3DCOLOR_XRGB(
		(int)(textColor[0] * 255),
		(int)(textColor[1] * 255),
		(int)(textColor[2] * 255)));

	std::ostringstream ammoInfo;

	ammoInfo << std::to_string(player.weapon.ammoInfo.clip < 0 ? 0 : player.weapon.ammoInfo.clip) << '/' << std::to_string(player.weapon.ammoInfo.reserve);

	m_pGfx->drawString(
		ammoInfo.str(),
		x + width + 5,
		y + height / 10,
		D3DCOLOR_XRGB(
		(int)(textColor[0] * 255), 
		(int)(textColor[1] * 255),
		(int)(textColor[2] * 255)));
}

float* Wallhack::getBoxColor() noexcept
{
	return boxColor;
}

float* Wallhack::getTextColor() noexcept
{
	return textColor;
}

