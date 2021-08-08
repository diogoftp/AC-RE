#pragma once
#include "pch.h"
#include "offsets.hpp"

#define STR_MERGE_IMPL(x, y)		x##y
#define STR_MERGE(x, y)				STR_MERGE_IMPL(x, y)
#define MAKE_PAD(size)				BYTE STR_MERGE(__pad__, __COUNTER__) [ size ]
#define DEFINE_MEMBER_N(x, offset)	struct { MAKE_PAD(offset); x; }
#define DEFINE_MEMBER_0(x, y)		struct { x; }

class Vec3 {
public:
	Vec3();
	Vec3(float _x, float _y, float _z);
	Vec3(float* ptr);
	std::string toString() const;
	float x;
	float y;
	float z;

	Vec3 operator+(const Vec3& vec);
	Vec3 operator-(const Vec3& vec);
	Vec3 operator*(const Vec3& vec);
	Vec3 operator/(const Vec3& vec);
	Vec3 operator=(const Vec3& vec);
};

class Entity_old {
public:
	char _pad0[4]; // 0x0
	Vec3 pos; // 0x4
	char _pad1[232]; // 0x10
	int health; // 0xF8
	char _pad2[84]; // 0xFC
	int ammo; // 0x150
	char _pad3[209]; // 0x154
	char name[24]; // 0x225
};

class Entity {
public:
	union {
		DEFINE_MEMBER_N(Vec3 pos, Offsets::pos);
		DEFINE_MEMBER_N(int health, Offsets::health);
		DEFINE_MEMBER_N(int ammo, Offsets::ammo);
		DEFINE_MEMBER_N(char name[32], Offsets::name);
	};
};
