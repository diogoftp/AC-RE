#include "pch.h"
#include "hooks.hpp"

bool detour32(void* targetAddr, void* myFunc, const int len) {
	if (len < 5) return false;

	DWORD curProtection;
	VirtualProtect(targetAddr, len, PAGE_EXECUTE_READWRITE, &curProtection);

	memset(targetAddr, 0x90, len);

	intptr_t relativeAddr = ((intptr_t)myFunc - (intptr_t)targetAddr) - 5;

	*(BYTE*)targetAddr = 0xE9;
	*(intptr_t*)((intptr_t)targetAddr + 1) = relativeAddr;

	DWORD temp;
	VirtualProtect(targetAddr, len, curProtection, &temp);

	return true;
}

char* tramp32(void* targetAddr, void* myFunc, const int len) {
	if(len < 5) return 0;

	void* gateway = VirtualAlloc(0, len + 5, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

	memcpy(gateway, targetAddr, len);

	intptr_t gatewayRelativeAddr = ((intptr_t)targetAddr - (intptr_t)gateway) - 5;

	*(char*)((intptr_t)gateway + len) = 0xE9;

	*(intptr_t*)((intptr_t)gateway + len + 1) = gatewayRelativeAddr;

	detour32(targetAddr, myFunc, len);

	return (char*)gateway;
}

TrampHook32::TrampHook32(void* _targetAddr, void* _myFunc, const int _len) {
	targetAddr = _targetAddr;
	myFunc = _myFunc;
	len = _len;
	active = false;
	memcpy(oBytes, (void*)targetAddr, 5);
}

TrampHook32::~TrampHook32() {
	unHook();
}

char* TrampHook32::tramp() {
	char* hook = tramp32(targetAddr, myFunc, len);
	if (hook != 0) {
		active = true;
	}
	return hook;
}

bool TrampHook32::hook() {
	if (detour32(targetAddr, myFunc, len)) {
		active = true;
		return true;
	}
	return false;
}

void TrampHook32::unHook() {
	DWORD curProtection;
	VirtualProtect(targetAddr, len, PAGE_EXECUTE_READWRITE, &curProtection);

	memcpy(targetAddr, oBytes, len);

	DWORD temp;
	VirtualProtect(targetAddr, len, curProtection, &temp);

	active = false;
}
