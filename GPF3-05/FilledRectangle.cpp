#include <algorithm>
#include "FilledRectangle.h"

FilledRectangle::FilledRectangle(float x, float y, int w, int h, float spx, float spy)
	: Rectangle(x, y, w, h, spx, spy)
{
}

//  buff フレームバッファの先頭アドレス
//  width, height フレームバッファの高さと横幅
void FilledRectangle::draw(unsigned char* buff, int width, int height)
{
	if (isActive) {
		unsigned char* pixel;
		sx = std::clamp(sx, 0, width);
		sy = std::clamp(sy, 0, height);
		ex = std::clamp(ex, 0, width);
		ey = std::clamp(ey, 0, height);
		for (int i = sy; i < ey; i++) { // 縦方向のループ
			pixel = buff + i * width * 3 + sx * 3; // フレームバッファ上で1行書き始める位置
			for (int j = sx; j < ex; j++) { // 横方向のループ
				*pixel++ = color[0]; // B
				*pixel++ = color[1]; // G
				*pixel++ = color[2]; // R
			}
		}
	}
}
