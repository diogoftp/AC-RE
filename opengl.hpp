#pragma once
#include "pch.h"
#pragma comment(lib, "OpenGL32.lib")
#include <Windows.h>
#include <gl/GL.h>
#include "structs.hpp"

namespace colors {
	const GLubyte red[3] = { 255, 0, 0 };
	const GLubyte green[3] = { 0, 255, 0 };
}

namespace GL {
	void SetupOrtho();
	void RestoreGL();
	void DrawFilledRect(float x, float y, float width, float height, const GLubyte color[3]);
	void DrawOutline(float x, float y, float width, float height, float lineWidth, const GLubyte color[3]);

	class Font {
	public:
		bool bBuilt = false;
		unsigned int base;
		HDC hdc = nullptr;
		int height;
		int width;

		void Build(int _width, int _height);
		void Print(float x, float y, const unsigned char color[3], const char* format, ...);

		Vec3 centerText(float x, float y, float width, float height, float textWidth, float textHeight);
		float centerText(float pos, size_t width, int FONT_WIDTH);
	};
}
