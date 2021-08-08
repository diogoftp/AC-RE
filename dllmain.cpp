#include "pch.h"
#include "structs.hpp"
#include <iostream>
#include <string>

DWORD WINAPI LoopThread(HMODULE hModule) {
	AllocConsole();
	FILE* f;
	freopen_s(&f, "CONOUT$", "w", stdout);

	uintptr_t gameBase = (uintptr_t)GetModuleHandle(L"ac_client.exe");
	Entity* localPlayer = *(Entity**)(gameBase + Offsets::localPlayer);
	uintptr_t entityList = *(uintptr_t*)(gameBase + Offsets::entityList);

	while (!GetAsyncKeyState(VK_INSERT)) {
		int playerNum = *(int*)(gameBase + Offsets::playerNum);

		system("cls");
		std::cout << "Number of players: " << playerNum << std::endl;
		std::cout << "localPlayer:" << std::endl;
		std::cout << "\tName: " << localPlayer->name << std::endl;
		std::cout << "\tHealth: " << localPlayer->health << std::endl;
		std::cout << "\tAmmo: " << localPlayer->ammo << std::endl;
		std::cout << "\tPosition: " << localPlayer->pos.toString() << std::endl;

		if (localPlayer->ammo < 5) localPlayer->ammo = 20;

		std::cout << "Entities:" << std::endl;
		int index;
		for (index = 1; index < playerNum; index++) {
			Entity* entity = *(Entity**)(entityList + 0x4 * index);
			if (!entity) continue;
			std::ios_base::fmtflags f(std::cout.flags());
			std::cout << "Name: " << std::left << std::setw(20) << entity->name;
			std::cout.flags(f);
			f = (std::cout.flags());
			std::cout << " Health: " << std::left << std::setw(3) << entity->health;
			std::cout.flags(f);
			f = (std::cout.flags());
			std::cout << " Ammo: " << std::left << std::setw(3) << entity->ammo;
			std::cout.flags(f);
			std::cout << " Position: " << entity->pos.toString() << std::endl;
		}

		Sleep(50);
	}

	if (f != NULL) fclose(f);
	FreeConsole();
	FreeLibraryAndExitThread(hModule, 0);
	return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hModule);
		if (auto handle = CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)LoopThread, hModule, 0, nullptr)) {
			CloseHandle(hModule);
		}
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
