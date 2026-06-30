#pragma once

#include	<sdl_image.h>	//SDL_IMageを使う為にインクルードする
#include	<SDL_ttf.h>
#include "Rectangle.h"

class Text : public Rectangle
{
public:
	Text(const char *fontName, int size, bool blended = false);
	~Text();

	void setText(const char* text);
	void draw(unsigned char* buff, int width, int height);

private:
	TTF_Font* font;
	SDL_Surface* image;				//画像を入れる用
	bool blended; // 文字列の描画方法（true アンチエイリアスあり、false アンチエイリアスなし）

	void drawSolid(unsigned char* buff, int width, int height);
	void drawBlended(unsigned char* buff, int width, int height);
};

