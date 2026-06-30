#include "Text.h"
#include "stdexcept"

Text::Text(const char* fontName, int size, bool blended)
	: image(nullptr), font(nullptr), blended(blended)
{
	if (TTF_Init() == -1)//初期化
	{
		throw std::runtime_error("failed to initialzie TTF.");
	}
	if ((font = TTF_OpenFont(fontName, size)) == NULL)//フォント取得
	{
		throw std::runtime_error("failed to open the font.");
	}
}

Text::~Text()
{
	if (image) {
		SDL_FreeSurface(image); // 文字列書き込み用のサーフェスを破棄
	}
	if (font) {
		TTF_CloseFont(font);
	}
}

void Text::setText(const char* text)
{
	if (image != nullptr) {
		SDL_FreeSurface(image); // 文字列書き込み用のサーフェスを破棄
		image = nullptr;
	}
	SDL_Color textColor = { color[0],color[1],color[2]};	// 文字の色
	if (blended) {
		image = TTF_RenderUTF8_Blended(font, text, textColor); // アンチエイリアスありで文字列を描画
	}
	else {
		image = TTF_RenderUTF8_Solid(font, text, textColor); // アンチエイリアスなしで文字列を描画
	}
	setSize(image->w, image->h);
}

void Text::draw(unsigned char* buff, int width, int height)
{
	if (blended) {
		drawBlended(buff, width, height);
	}
	else {
		drawSolid(buff, width, height);
	}
}

void Text::drawSolid(unsigned char* buff, int width, int height)
{
	unsigned char* pixel;
	for (int i = sy; i < ey; i++) { // 縦方向のループ
		pixel = buff + i * width * 3 + sx * 3; // フレームバッファ上で1行書き始める位置
		unsigned char* img = (unsigned char*)image->pixels + (i - sy) * image->pitch;
		for (int j = sx; j < ex; j++) { // 横方向のループ
			unsigned char c = *img++;
			if (c == 0) {
				pixel += 3; // 完全に透明なピクセルは描画しない
			}
			else {
				*pixel++ = image->format->palette->colors[c].b; // B
				*pixel++ = image->format->palette->colors[c].g; // G
				*pixel++ = image->format->palette->colors[c].r; // R
			}
		}
	}
}

void Text::drawBlended(unsigned char* buff, int width, int height)
{
	unsigned char* pixel;
	for (int i = sy; i < ey; i++) { // 縦方向のループ
		pixel = buff + i * width * 3 + sx * 3; // フレームバッファ上で1行書き始める位置
		unsigned char* img = (unsigned char*)image->pixels + (i - sy) * image->pitch;
		for (int j = sx; j < ex; j++) { // 横方向のループ
			unsigned char b = *img++;
			unsigned char g = *img++;
			unsigned char r = *img++;
			float a = (float)(*img++) / 255.0f;
			*pixel++ = b * a + *pixel * (1 - a); // B
			*pixel++ = g * a + *pixel * (1 - a); // G
			*pixel++ = r * a + *pixel * (1 - a); // R
		}
	}
}