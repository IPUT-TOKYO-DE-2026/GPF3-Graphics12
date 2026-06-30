#pragma once
#include "GameObject.h"
#include "Circle.h"

class Rectangle : public GameObject {
public:
	Rectangle();
	Rectangle(float x, float y, int w, int h, float spx, float spy);
	virtual ~Rectangle() {}

	void setSize(int w, int h);
	void rotate(float angle); // 角度を指定して回転（angleはラジアン）
	void calcBoundingBox();
	virtual void draw(unsigned char* buff, int width, int height) = 0;
	bool isHit(const Circle& c);
	bool isHit(const Rectangle& r);

	int sizeW;   // 幅
	int sizeH;   // 高さ
	float angle;  // 回転角度（ラジアン）
};
