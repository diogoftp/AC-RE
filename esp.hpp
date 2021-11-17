#pragma once
#include "pch.h"
#include "structs.hpp"
#include "opengl.hpp"

const float PLAYER_HEIGHT = 5.25f;
const float PLAYER_WIDTH = 2.0f;
const float PLAYER_ASPECT_RATIO = PLAYER_HEIGHT / PLAYER_WIDTH;
const int ESP_FONT_WIDTH = 9;
const int ESP_FONT_HEIGHT = 15;

void DrawESP(Entity* localPlayer, Entity* enemy, int viewPort[4], Vec3 screen, GL::Font glFont);
