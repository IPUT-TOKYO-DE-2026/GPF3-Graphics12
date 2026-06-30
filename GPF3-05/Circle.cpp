#include <math.h>
#include <algorithm>

#include "Circle.h"

Circle::Circle()
	: radius(0)
{
}

Circle::Circle(float x, float y, int r, float spx, float spy)
	: GameObject(x, y, spx, spy), radius(r)
{
	calcBoundingBox();
}

void Circle::setCenter(float x, float y)
{
	centerX = x;
	centerY = y;

	calcBoundingBox();
}

void Circle::setRadius(int r)
{
	radius = r;

	calcBoundingBox();
}

void Circle::move()
{
	centerX += speedX;
	centerY += speedY;

	calcBoundingBox();
}

void Circle::calcBoundingBox()
{
	sx = centerX - radius;
	sy = centerY - radius;
	ex = centerX + radius;
	ey = centerY + radius;
}

//  buff フレームバッファの先頭アドレス
//  width, height フレームバッファの高さと横幅
void Circle::draw(unsigned char* buff, int width, int height)
{
	if (isActive) {
		unsigned char* pixel;
		const float squaredRadius = radius * radius; // 半径の二乗
		sx = std::clamp(sx, 0, width);
		sy = std::clamp(sy, 0, height);
		ex = std::clamp(ex, 0, width);
		ey = std::clamp(ey, 0, height);

		for (int y = sy; y <= ey; y++) { // 縦方向のループ
			float squaredY = y -  centerY;	// Y軸の中心からの距離
			squaredY *= squaredY;  // 二乗しておく
			pixel = buff + y * width * 3 + sx * 3; // フレームバッファ上で1行書き始める位置
			for (int x = sx; x <= ex; x++) { // 横方向のループ
				float squaredX = x - centerX;	// X軸の中心からの距離
				squaredX *= squaredX;  // 二乗しておく
				if (squaredY + squaredX <= squaredRadius) { // 距離が半径以下ならば（二乗どうしで比較）
					// 現在のX,Yで示す位置は円の内側として色を置く
					*pixel++ = color[0];  // B
					*pixel++ = color[1];  // G
					*pixel++ = color[2];  // R
				}
				else {
					pixel += 3;	// 現在のX,Yで示す位置は円の外側（色は置かず次のピクセルに移る）
				}
			}
		}
	}
}

bool Circle::isHit(const Circle& c)
{
	// X軸とY軸のそれぞれの距離（差分）を計算
	float dx = c.centerX - centerX;
	float dy = c.centerY - centerY;

	// 中心点間の距離の2乗を計算
	float distanceSquared = (dx * dx) + (dy * dy);
	// 半径の合計の2乗を計算
	float radiusSum = radius + c.radius;
	float radiusSumSquared = radiusSum * radiusSum;

	// 距離の2乗が半径の2乗以下なら衝突している
	if (distanceSquared <= radiusSumSquared) {
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