#include <Windows.h>
#include <iostream>
#include <Psapi.h>
#include <string>
#include "MenuRollStats.h"
#include <json/json.h>
#include <fstream>
#include <vector>
#include "CharacterJsonStruct.h"

bool GetMagicite(HANDLE &, HMODULE &);

int main()
{
	HANDLE magiciteProcess;
	HMODULE monoDll;

	if (!GetMagicite(magiciteProcess, monoDll))
	{
		std::cout << "Magicite was not found...";
		std::cin.get();
		return 1;
	}

	unsigned int baseAddress = MenuRollBaseAddress(magiciteProcess, monoDll);

	std::ifstream characterJson("characters.json");

	Json::Value root;
	Json::Reader parse;
	if (parse.parse(characterJson, root, false))
	{
		int characterCount = root.size();
		std::cout << "Found " << characterCount << " characters:\n";
		std::vector<CharacterJson> characters;
		for (int i = 0; i < characterCount; i++)
		{
			CharacterJson addCharacter;

			addCharacter.name = root[i].get("name", "NA").asString();
			addCharacter.attack = root[i].get("attack", 1).asInt();
			addCharacter.dex = root[i].get("dex", 1).asInt();
			addCharacter.hp = root[i].get("hp", 1).asInt();
			addCharacter.magic = root[i].get("magic", 1).asInt();
			addCharacter.trait1 = root[i].get("trait1", 1).asInt();
			addCharacter.trait2 = root[i].get("trait2", 1).asInt();
			characters.push_back(addCharacter);

			std::cout << "{" << i + 1 << "}" << " " << addCharacter.name<<"\n";
		}

		selection_choice:
		std::cout << "Select a character (enter character's number and press enter): ";
		int selection;
		std::cin >> selection;

		selection--; //selections start at 1 graphically

		if (selection >= characterCount || selection < 0)
		{
			std::cout << "Selection doesn't exist\n";
			goto selection_choice;
		}

		
		CharacterJson characterData = characters[selection];
		MenuRollStats stats;

		ReadProcessMemory(magiciteProcess, (LPCVOID)baseAddress, &stats, sizeof(stats), NULL);

		stats.Attack = characterData.attack;
		stats.Magic = characterData.magic;
		stats.Health= characterData.hp;
		stats.Dex = characterData.dex;
		stats.FirstTrait = characterData.trait1;
		stats.SecondTrait = characterData.trait2;

		WriteProcessMemory(magiciteProcess, (LPVOID)baseAddress, &stats, sizeof(MenuRollStats), NULL);

		std::cout << "Character changed. (Stats wont appear changed until you start the game).";
		std::cin >> selection;
	}
	else
	{
		std::cout << "Error parsing json characters.json\n";
		std::cout << parse.getFormattedErrorMessages();
		std::cin.get();
	}



	return 0;
}

bool GetMagicite(HANDLE &process, HMODULE &monoModule)
{
	const int processLimit = 1000;
	const int moduleLimit = 5000;
	DWORD processes[processLimit];
	DWORD size;
	EnumProcesses(processes, processLimit, &size);

	for (int i = 0; i < size / sizeof(DWORD); i++)
	{
		HANDLE processHandle = OpenProcess(PROCESS_ALL_ACCESS, TRUE, processes[i]);

		WCHAR title[MAX_PATH];
		if (GetModuleFileNameEx(processHandle, 0, title, MAX_PATH))
		{
			std::wstring titleStr = title;
			if (titleStr.find(L"Magicite.exe") != std::wstring::npos)
			{
				process = processHandle;

				HMODULE modules[moduleLimit];
				DWORD moduleBytes;
				EnumProcessModules(processHandle, modules, moduleLimit, &moduleBytes);

				for (int j = 0; j < moduleBytes / sizeof(HMODULE); j++)
				{
					if (GetModuleFileNameEx(processHandle, modules[j], title, MAX_PATH))
					{
						titleStr = title;
						if (titleStr.find(L"mono") != std::wstring::npos)
						{
							monoModule = modules[j];
							return true;
						}
					}
				}
			}
		}
	}
	return false;
}