#include <algorithm>
#include "Sprite.h"

std::unordered_map<std::string, std::shared_ptr<SDL_Surface>> Sprite::imageCache; // 画像キャッシュ

Sprite::Sprite(const char* fileName)
{
	loadImage(fileName);
}

Sprite::~Sprite()
{

}

bool Sprite::loadImage(const char* filename)
{
	if (imageCache.find(filename) != imageCache.end()) { // キャッシュに画像が存在する場合
		image = imageCache[filename]; // ファイル名をキーにしてキャッシュから画像を取得
	} else {
		// 画像を読み込み、SDL_Surfaceを管理する共有ポインタを作成
		// SDL_FreeSurfaceをデリータとして指定して、delete時にSDL_Surfaceのメモリを自動的に解放する
		image = std::shared_ptr<SDL_Surface>(SDL_LoadBMP(filename), SDL_FreeSurface); 
		if (image == nullptr) {
			return false;
		}
		imageCache[filename] = image; // ファイル名をキーにしてキャッシュに保存
	}

	//画像加工部
	int imageW = image->w;
	int imageH = image->h;
	bytesPerPixel = image->format->BytesPerPixel;
	setSize(imageW, imageH);

	return true;
}

// アニメーション用の画像フレームを追加する
bool Sprite::addImageFrame(const char* filename)
{
	if (imageCache.find(filename) != imageCache.end()) {
		frames.push_back(imageCache[filename]);
	} else {
		std::shared_ptr<SDL_Surface> frame(SDL_LoadBMP(filename), SDL_FreeSurface);
		if (frame == nullptr) {
			return false;
		}
		imageCache[filename] = frame;
		frames.push_back(frame);
	}
	return true;
}

//  buff フレームバッファの先頭アドレス
//  width, height フレームバッファの高さと横幅
void Sprite::draw(unsigned char* buff, int width, int height)
{
	if (frames.size() > 0) { // アニメーションフレームがある場合
		// フレームアニメーションの処理
		float d = animDuration / frames.size(); // 1フレームあたりの表示時間
		int currentFrame = static_cast<int>(currentTime / d) % frames.size(); // 現在のフレーム番号を計算
		image = frames[currentFrame]; // 現在のフレームの画像を設定
		bytesPerPixel = image->format->BytesPerPixel;
		setSize(image->w, image->h);
	}
	if (isActive) {
		if (fabsf(angle) > 0.0001f) { // fabsf 絶対値
			draw(angle, buff, width, height);
		} else if (bytesPerPixel == 3) {
			int clipSX = std::clamp(sx, 0, width);
			int clipSY = std::clamp(sy, 0, height);
			int clipEX = std::clamp(ex, 0, width);
			int clipEY = std::clamp(ey, 0, height);
			unsigned char* pixel;
			unsigned char* img;
			for (int i = clipSY; i < clipEY; i++) { // 縦方向のループ
				unsigned char* img = (unsigned char*)image->pixels + (i - sy) * sizeW * bytesPerPixel + (clipSX - sx) * bytesPerPixel;
				pixel = buff + i * width * 3 + clipSX * 3; // フレームバッファ上で1行書き始める位置
				for (int j = clipSX; j < clipEX; j++) { // 横方向のループ
					*pixel++ = *img++; // B
					*pixel++ = *img++; // G
					*pixel++ = *img++; // R
				}
			}
		}
		else if (bytesPerPixel == 4) {
			drawWithAlpha(buff, width, height);
		}
	}
}

void Sprite::drawWithAlpha(unsigned char* buff, int width, int height)
{
	unsigned char* pixel;
	unsigned char* img;
	int clipSX = std::clamp(sx, 0, width);
	int clipSY = std::clamp(sy, 0, height);
	int clipEX = std::clamp(ex, 0, width);
	int clipEY = std::clamp(ey, 0, height);
	for (int i = clipSY; i < clipEY; i++) { // 縦方向のループ
		img = (unsigned char*)image->pixels + (i - sy) * sizeW * bytesPerPixel + (clipSX - sx) * bytesPerPixel;
		pixel = buff + i * width * 3 + clipSX * 3; // フレームバッファ上で1行書き始める位置
		for (int j = clipSX; j < clipEX; j++) { // 横方向のループ
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

void Sprite::draw(float radian, unsigned char* buff, int width, int height)
{
	// 回転ありの場合は、回転後の矩形を計算して描画する
	// 回転後の矩形の左上と右下の座標を計算する
	float cosA = cos(angle);
	float sinA = sin(angle);
	float diag = sqrt(static_cast<float>(sizeW * sizeW + sizeH * sizeH)) / 2.0f;
	int clipSX = std::clamp((int)(centerX - diag), 0, width);
	int clipSY = std::clamp((int)(centerY - diag), 0, height);
	int clipEX = std::clamp((int)(centerX + diag), 0, width);
	int clipEY = std::clamp((int)(centerY + diag), 0, height);
	float srcHalfW = sizeW / 2.0f;
	float srcHalfH = sizeH / 2.0f;

	if (bytesPerPixel == 3) {
		for (int y = clipSY; y < clipEY; y++) { // 縦方向のループ
			for (int x = clipSX; x < clipEX; x++) { // 横方向のループ
				// 出力バッファの中心座標からの相対位置
				float dx = x - centerX;
				float dy = y - centerY;

				// 逆回転させて、回転前（元画像）のローカル座標に戻す
				float localX = dx * cosA + dy * sinA;
				float localY = -dx * sinA + dy * cosA;

				// 元画像の中心からのインデックスに変換
				float srcX = localX + srcHalfW;
				float srcY = localY + srcHalfH;

				// 元画像の範囲（有効なピクセル）に収まっているか判定
				int texX = static_cast<int>(floor(srcX));
				int texY = static_cast<int>(floor(srcY));

				if (texX >= 0 && texX < sizeW && texY >= 0 && texY < sizeH) {
					// 出力バッファと元画像のメモリインデックスを計算
					int destIdx = (y * width + x) * 3;
					int srcIdx = (texY * sizeW + texX) * bytesPerPixel;

					// チャンネル数分（RGB）のピクセルデータをコピー
					for (int c = 0; c < bytesPerPixel; ++c) {
						buff[destIdx + c] = ((unsigned char*)image->pixels)[srcIdx + c];
					}
				}
			}
		}
	}
	else if (bytesPerPixel == 4) {
		for (int y = clipSY; y < clipEY; y++) { // 縦方向のループ
			for (int x = clipSX; x < clipEX; x++) { // 横方向のループ
				// 出力バッファの中心座標からの相対位置
				float dx = x - centerX;
				float dy = y - centerY;

				// 逆回転させて、回転前（元画像）のローカル座標に戻す
				float localX = dx * cosA + dy * sinA;
				float localY = -dx * sinA + dy * cosA;

				// 元画像の中心からのインデックスに変換
				float srcX = localX + srcHalfW;
				float srcY = localY + srcHalfH;

				// 元画像の範囲（有効なピクセル）に収まっているか判定
				int texX = static_cast<int>(floor(srcX));
				int texY = static_cast<int>(floor(srcY));

				if (texX >= 0 && texX < sizeW && texY >= 0 && texY < sizeH) {
					// 出力バッファと元画像のメモリインデックスを計算
					int destIdx = (y * width + x) * 3;
					int srcIdx = (texY * sizeW + texX) * bytesPerPixel;

					float a = (float)(((unsigned char*)image->pixels)[srcIdx + 3]) / 255.0f;
					// チャンネル数分（RGB/RGBA）のピクセルデータをコピー
					for (int c = 0; c < 3; ++c) {
						buff[destIdx + c] = ((unsigned char*)image->pixels)[srcIdx + c] * a + buff[destIdx + c] * (1 - a);
						//  b * a + *pixel * (1 - a); // B
					}
				}
			}
		}
	}
}
