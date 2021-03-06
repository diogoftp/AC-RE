#include "pch.h"
#include "structs.hpp"
#include "hooks.hpp"
#include "opengl.hpp"
#include "funcs.hpp"
#include "esp.hpp"
#include <iostream>
#include <string>

DWORD damageJumpAddr;
void __declspec(naked) hookedDamage() {
	__asm {
		add[ebx + 04], 50
		mov eax, edi
		jmp[damageJumpAddr]
	}
}

// Old stuff
/*void hwglSwapBuffers(_In_ HDC hDc) {
	std::cout << "Hooked" << std::endl;
}
DWORD owglSwapBuffers;
void __declspec(naked) wglSwapBuffersTrampoline() {
	__asm {
		pushfd
		pushad
		call hwglSwapBuffers
		popad
		popfd
		mov edi, edi
		push ebp
		mov ebp, esp
		jmp [owglSwapBuffers]
	}
}*/

GL::Font glFont;
uintptr_t gameBase = (uintptr_t)GetModuleHandle(L"ac_client.exe");

typedef int(__stdcall* twglSwapBuffers)(HDC hDC);
twglSwapBuffers owglSwapBuffers;
int __stdcall hwglSwapBuffers(HDC hDC) {
	HDC currentHDC = wglGetCurrentDC();
	if (!glFont.bBuilt || currentHDC != glFont.hdc) {
		glFont.Build(ESP_FONT_WIDTH, ESP_FONT_HEIGHT);
	}

	GL::SetupOrtho();

	Entity* localPlayer = *(Entity**)(gameBase + Offsets::localPlayer);
	int playerNum = *(int*)(gameBase + Offsets::playerNum);
	uintptr_t entityList = *(uintptr_t*)(gameBase + Offsets::entityList);
	float* viewMatrix = (float*)(gameBase + Offsets::viewMatrix);
	int viewPort[4];
	glGetIntegerv(GL_VIEWPORT, viewPort);
	int index;
	for (index = 1; index < playerNum; index++) {
		Entity* entity = *(Entity**)(entityList + 0x4 * index);
		if (!entity) continue;
		Vec3 screen;
		Vec3 entpos = entity->head;
		entpos.z += 0.75f;
		if (W2S(entpos, screen, viewMatrix, viewPort[2], viewPort[3])) {
			DrawESP(localPlayer, entity, viewPort, screen, glFont);
		}
	}

	GL::RestoreGL();
	return owglSwapBuffers(hDC);
}

DWORD WINAPI LoopThread(HMODULE hModule) {
	AllocConsole();
	FILE* f;
	freopen_s(&f, "CONOUT$", "w", stdout);

	Entity* localPlayer = *(Entity**)(gameBase + Offsets::localPlayer);
	uintptr_t entityList = *(uintptr_t*)(gameBase + Offsets::entityList);

	DWORD hookAddr = (DWORD)gameBase + 0x29D1F;
	int hookLength = 5;
	damageJumpAddr = hookAddr + hookLength;
	TrampHook32* damageHook = new TrampHook32((void*)hookAddr, hookedDamage, hookLength);
	
	DWORD sbAddr = (DWORD)GetProcAddress(GetModuleHandle(L"opengl32.dll"), "wglSwapBuffers");
	TrampHook32* drawHook = new TrampHook32((void*)sbAddr, hwglSwapBuffers, 5);

	// Old stuff
	//TrampHook32* drawHook = new TrampHook32((void*)sbAddr, wglSwapBuffersTrampoline, 5);
	//owglSwapBuffers = (DWORD)sbAddr + 5;
	//drawHook->hook();
	
	while (!GetAsyncKeyState(VK_INSERT)) {
		int playerNum = *(int*)(gameBase + Offsets::playerNum);

		system("cls");
		std::cout << "Hooks: " << damageHook->active << std::endl;
		std::cout << "Number of players: " << playerNum << std::endl;
		std::cout << "localPlayer:" << std::endl;
		std::cout << "\tName: " << localPlayer->name << std::endl;
		std::cout << "\tHealth: " << localPlayer->health << std::endl;
		std::cout << "\tAmmo: " << localPlayer->ammo << std::endl;
		std::cout << "\tPosition: " << localPlayer->pos.toString() << std::endl;

		if (localPlayer->ammo < 5) localPlayer->ammo = 20;

		if (GetAsyncKeyState(VK_DELETE)) {
			if (!damageHook->active) damageHook->hook();
			else damageHook->unHook();
			if (!drawHook->active) owglSwapBuffers = (twglSwapBuffers)drawHook->tramp();
			else drawHook->unHook();
		}

		if (GetAsyncKeyState(0x12)) {
			int index;
			for (index = 1; index < playerNum; index++) {
				Entity* entity = *(Entity**)(entityList + 0x4 * index);
				if (!entity) continue;
				// get closest enemy
				Vec3 screen;
				int viewport[4];
				glGetIntegerv(GL_VIEWPORT, viewport);
				float* matrix = (float*)(gameBase + Offsets::viewMatrix);
				if (W2S(entity->head, screen, matrix, viewport[2], viewport[3])) {
					// do aimbot
				}
				break;
			}
		}

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

	delete damageHook;
	delete drawHook;

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
