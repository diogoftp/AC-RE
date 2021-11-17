#include "pch.h"
#include "funcs.hpp"

bool W2S(Vec3 pos, Vec3& screen, float matrix[16], int width, int height) {
	float cx, cy, cz, cw;

	cx = pos.x * matrix[0] + pos.y * matrix[4] + pos.z * matrix[8] + matrix[12];
	cy = pos.x * matrix[1] + pos.y * matrix[5] + pos.z * matrix[9] + matrix[13];
	cz = pos.x * matrix[2] + pos.y * matrix[6] + pos.z * matrix[10] + matrix[14];
	cw = pos.x * matrix[3] + pos.y * matrix[7] + pos.z * matrix[11] + matrix[15];

	if (cw < 0.1f) return false;

	Vec3 NDC;
	NDC.x = cx / cw;
	NDC.y = cy / cw;
	NDC.z = cz / cw;

	screen.x = (width / 2 * NDC.x) + (NDC.x + width / 2);
	screen.y = -(height / 2 * NDC.y) + (NDC.y + height / 2);
	screen.z = 0;

	return true;
}
