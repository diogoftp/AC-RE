#include "pch.h"
#include "esp.hpp"
#include "opengl.hpp"

void DrawESP(Entity* localPlayer, Entity* enemy, int viewPort[4], Vec3 screen, GL::Font glFont) {
	float scale = (400 / localPlayer->pos.dist(enemy->pos)) * (viewPort[2] / 800);
	float x = screen.x - scale;
	float y = screen.y;
	float width = scale * 2;
	float height = scale * PLAYER_ASPECT_RATIO * 2;
	GL::DrawOutline(x, y, width, height, 2.0f, colors::red);
	float textX = glFont.centerText(screen.x, strlen(enemy->name), glFont.width);
	float textY = screen.y;
	glFont.Print(textX, textY, colors::green, "%s", enemy->name);
}
