#include "Rectangle.h"

#include <algorithm>

Rectangle::Rectangle()
	: angle(0)
{
	setSize(0, 0);
}

Rectangle::Rectangle(float x, float y, int w, int h, float spx, float spy)
	: angle(0), GameObject(x, y, spx, spy)
{
	setSize(w, h);
}

void Rectangle::calcBoundingBox()
{
	sx = centerX - sizeW / 2;
	sy = centerY - sizeH / 2;
	ex = sx + sizeW;
	ey = sy + sizeH;
}

void Rectangle::setSize(int w, int h)
{
	sizeW = w;
	sizeH = h;

	calcBoundingBox();
}

void Rectangle::rotate(float angle)
{
	this->angle = angle;
}

bool Rectangle::isHit(const Circle& c)
{
	if (!isActive) {
		return false; // アクティブでなければ当たっていないとする
	}

	// 円の中心に最も近い矩形上の点を求める
	float closestX = std::clamp((int)c.centerX, sx, ex);
	float closestY = std::clamp((int)c.centerY, sy, ey);

	// 最短点と円の中心との距離（の2乗）を計算
	float distX = c.centerX - closestX;
	float distY = c.centerY - closestY;
	float distanceSquared = (distX * distX) + (distY * distY);

	float radiusSquared = c.radius * c.radius; // 円の半径の2乗

	// 距離の2乗が半径の2乗以下なら衝突している
	if (distanceSquared <= radiusSquared) {
		if (isAlreadyHit) {
			return false;
		}
		else {
			isAlreadyHit = true;
			return true;
		}
	}
	else {
		isAlreadyHit = false;
	}

	return false;
}

bool Rectangle::isHit(const Rectangle& r)
{
	if (!isActive) {
		return false; // アクティブでなければ当たっていないとする
	}

	// 完全に離れている条件のいずれかが満たされていれば衝突していない
	if (ex <= r.sx || // AがBの左側にある
		sx >= r.ex || // AがBの右側にある
		ey <= r.sy || // AがBの上側にある
		sy >= r.ey)   // AがBの下側にある
	{
		isAlreadyHit = false;
		return false;
	}
	else {
		if (isAlreadyHit) {
			return false;
		}
		else {
			isAlreadyHit = true;
			return true;
		}
	}
}
