#pragma once
#include "pch.h"

bool detour32(void* targetAddr, void* func, const int len);
char* tramp32(void* targetAddr, void* myFunc, const int len);

class TrampHook32 {
public:
	TrampHook32(void* _targetAddr, void* _myFunc, const int _len);
	~TrampHook32();

	void* targetAddr;
	void* myFunc;
	char oBytes[32]{ 0 };
	int len;
	bool active;

	char* tramp();
	bool hook();
	void unHook();
};
