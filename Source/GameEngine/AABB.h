#pragma once
#ifndef AABB_H
#define AABB_H

namespace GameEngine
{
	struct AABB
	{
		inline AABB() :x(0.0f), y(0.0f), w(0), h(0) {}
		float x, y;
		int w, h;
		inline void setX(float x) { this->x = x; }
		inline void setY(float y) { this->y = y; }
		inline void setW(int w) { this->w = w; }
		inline void setH(int h) { this->h = h; }
	};
}

#endif