#pragma once
#include "Rectangle.h"
class FilledRectangle : public Rectangle
{
public:
	FilledRectangle() {};
	FilledRectangle(float x, float y, int w, int h, float spx, float spy);

	void draw(unsigned char* buff, int width, int height);
};

