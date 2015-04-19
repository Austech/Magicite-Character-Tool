#pragma once
#include <Windows.h>

struct MenuRollStats
{
	enum Traits
	{
		WoodCutter = 1,
		Miner = 2,
		Gatherer = 3,
		BigEater = 4,
		PotionBrewer = 4,
		Artisan = 5,
		Aggressive = 6,
		Defensive = 7,
		Swift = 8,
		Healthy = 9,
		Intellegent = 11,
		Lockmaster = 12,
	};
	unsigned int Health;
	unsigned int Attack;
	unsigned int Dex;
	unsigned int Magic;

	//0x8164 Bytes of unknown values
	unsigned char UnknownData[0x8164];

	int FirstTrait;
	int SecondTrait;
};

unsigned int MenuRollBaseAddress(const HANDLE &process, const HMODULE &mono);

/* Path To Menu Stat's Health Variable*/
//"mono.dll"+001F20AC -> 0x4 -> 0x58 -> 0x40 -> 0xD0