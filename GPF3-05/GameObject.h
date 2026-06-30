#pragma once

class GameObject {
public:
	GameObject();
	GameObject(float x, float y, float spx, float spy);
	virtual ~GameObject() {}

	void setColor(int b, int g, int r);
	void setSpeed(float x, float y);
	void setCenter(float x, float y);
	void move();
	virtual void draw(unsigned char* buff, int width, int height) = 0;
	virtual void calcBoundingBox() = 0;

	float centerX; // 中心 X座標
	float centerY; // 中心 Y座標
	float speedX;  // 移動時のX増加値（速度）
	float speedY;  // 移動時のY増加値（速度）

	int sx;      // 左端のX座標
	int sy;      // 上端のY座標
	int ex;      // 右端のX座標
	int ey;      // 下端のY座標

	bool isActive;
	bool isAlreadyHit;

	unsigned char color[3] = { 0, 0, 0 };
};

