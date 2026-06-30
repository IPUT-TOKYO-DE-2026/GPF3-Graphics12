#pragma once
#include "GameObject.h"

class Circle : public GameObject {
public:
	Circle();
	Circle(float x, float y, int r, float spx, float spy);

	void setCenter(float x, float y);
	void setRadius(int r);
	void move();
	void draw(unsigned char* buff, int width, int height);
	void calcBoundingBox();

	bool isHit(const Circle& c);

	int radius;  // 半径
};
