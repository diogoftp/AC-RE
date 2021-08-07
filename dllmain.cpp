#include "pch.h"
#include <iostream>

DWORD WINAPI LoopThread(HMODULE hModule) {
	AllocConsole();
	FILE* f;
	freopen_s(&f, "CONOUT$", "w", stdout);

	uintptr_t gameBase = (uintptr_t)GetModuleHandle(L"ac_client.exe");
	uintptr_t localPlayer = *(uintptr_t*)(gameBase + 0x10f4f4);
	uintptr_t entityList = *(uintptr_t*)(gameBase + 0x10f4f8);

	while (!GetAsyncKeyState(VK_INSERT)) {
		int health = *(int*)(localPlayer + 0xF8);
		int ammo = *(int*)(localPlayer + 0x150);
		char* name = (char*)(localPlayer + 0x225);
		float myX = *(float*)(localPlayer + 0x4);
		float myY = *(float*)(localPlayer + 0x8);
		float myZ = *(float*)(localPlayer + 0xC);

		system("cls");
		std::cout << "gameBase: " << std::hex << gameBase << std::dec << std::endl;
		std::cout << "localPlayer: " << std::hex << localPlayer << std::dec << std::endl;
		std::cout << "health: " << health << std::endl;
		std::cout << "ammo: " << ammo << std::endl;
		std::cout << "name: " << name << std::endl;
		std::cout << "position: (" << myX << ", " << myY << ", " << myZ << ")" << std::endl;

		int playerNum = *(int*)(gameBase + 0x10F500);
		std::cout << "playerNum: " << playerNum << std::endl;

		int index;
		void* entity = NULL;
		for (index = 1; index < playerNum; index++) {
			uintptr_t entity = *(uintptr_t*)(entityList + 0x4 * index);
			if (!entity) continue;
			int entityHealth = *(int*)(entity + 0xF8);
			char* entityName = (char*)(entity + 0x225);
			std::ios_base::fmtflags f(std::cout.flags());
			std::cout << "Name: " << std::left << std::setw(20) << entityName << " Health: " << entityHealth << std::endl;
			std::cout.flags(f);
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
