#include "MenuRollStats.h"
#include <iostream>

unsigned int MenuRollBaseAddress(const HANDLE &process, const HMODULE &mono)
{
	unsigned int buffer = (unsigned int)mono + 0x001F4944;

	ReadProcessMemory(process, (unsigned int*)buffer, &buffer, 4, NULL);
	buffer += 0x30;
	ReadProcessMemory(process, (unsigned int*)buffer, &buffer, 4, NULL);
	buffer += 0x150;
	ReadProcessMemory(process, (unsigned int*)buffer, &buffer, 4, NULL);
	buffer += 0x14;
	ReadProcessMemory(process, (unsigned int*)buffer, &buffer, 4, NULL);
	buffer += 0xa0;

	return buffer;
}