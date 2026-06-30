#pragma once
#include	<sdl_image.h>	//SDL_IMageを使う為にインクルードする

#include "Rectangle.h"

class Sprite : public Rectangle
{
public:
	Sprite() : image(nullptr), bytesPerPixel(0) {}
	Sprite(const char* fileName);
	~Sprite();

	bool loadImage(const char* filename);
	void draw(unsigned char* buff, int width, int height);
	void draw(float radian, unsigned char* buff, int width, int height);

	SDL_Surface* image;				//画像を入れる用
	int bytesPerPixel;  // 1ピクセル当たりのバイト数（3 or 4）

private:
	void drawWithAlpha(unsigned char* buff, int width, int height);
};
