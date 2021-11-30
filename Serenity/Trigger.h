#pragma once
#include "MemWiz.h"
#include "Player.h"
#include "csgo.hpp"
#include <memory>
#include <mutex>
#include <random>

class Trigger
{
	static std::mt19937 twister;
	Mem::MemWiz* manager;
	unsigned int lastAimedAt;

public:
	int randomDelayMaxms;
	Trigger(Mem::MemWiz* manager);
	void triggerbot();
	bool triggerActive();
	void randomShoot(int sleepTime);
};

