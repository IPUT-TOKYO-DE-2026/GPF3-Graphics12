#pragma once
#include	<sdl_image.h>	//SDL_IMageを使う為にインクルードする
#include <vector>
#include <unordered_map>
#include <string>
#include <memory>
#include <ctime>

#include "Rectangle.h"

class Sprite : public Rectangle
{
public:
	Sprite() : image(nullptr), bytesPerPixel(0) {}
	Sprite(const char* fileName);
	~Sprite();

	bool loadImage(const char* filename);
	bool addImageFrame(const char* filename); // 画像フレームを追加する
	void setAnimDuration(float duration) { animDuration = duration; } // アニメーションの総時間を設定
	void setTime(float time) { currentTime = time; } // ゲーム開始からの時間を設定
	void draw(unsigned char* buff, int width, int height);
	void draw(float radian, unsigned char* buff, int width, int height);


private:
	static std::unordered_map<std::string, std::shared_ptr<SDL_Surface>> imageCache; // 画像キャッシュ

	std::shared_ptr<SDL_Surface> image;				//画像を入れる用
	std::vector<std::shared_ptr<SDL_Surface>> frames;
	int bytesPerPixel;  // 1ピクセル当たりのバイト数（3 or 4）
	float animDuration = 0.1f; // アニメーションの総時間（秒）
	float currentTime; // ゲーム開始からの時間（秒）

	void drawWithAlpha(unsigned char* buff, int width, int height);
};
