#pragma once

#include <vector>

#include "FrameBufferEmulator.h"
#include "Circle.h"
#include "FilledRectangle.h"
#include "Sprite.h"
#include "Text.h"

class BlocksGame : public FrameBufferEmulator
{
public:
	BlocksGame();
	~BlocksGame() {}

	void initUser();
	void drawUser(unsigned char* buff, int mode, int keyLevel, int keyTrigger);

private:
	static constexpr int RectRowNum = 4; // ブロックの行数
	static constexpr int RectColumn = 9; // ブロックの列数
	static constexpr int RectNum = RectRowNum * RectColumn; // ブロックの総数
	static constexpr int RectWidth = 100; // ブロックの幅
	static constexpr int RectHeight = 40; // ブロックの高さ
	static constexpr int RectGap = 5; // ブロックの間

	int barX; // バーのX座標
	int barY; // バーのY座標
	int barSpeedX = 3; // バーの横方向の移動スピード

	int barRemaining;   // バーの残機
	int blockRemaining; // 残りのブロックの数
	int score;          // スコア

	float rot = 0.0f;

	std::vector<Circle> enemys;

	Circle ball; // ボール
	Circle pin;
	int scoreTable[RectNum]; // ブロックごとのスコアを保持する配列
	Sprite bar; // バーをスプライトとして定義

	unsigned char rectColor[RectRowNum][3] = { // ブロックの色（列ごとに設定）
		{0, 249, 253},
		{0, 253,   0},
		{0, 126, 253},
		{0,   0, 253},
	};

	std::vector<GameObject*> objects; // すべてのオブジェクトを保持する可変長配列
	Text text;
	Text remainingText;
	Text scoreText;

	bool isPlaying = false;
	Mix_Chunk* failedSound;
	Mix_Chunk* gameOverSound;
	Mix_Chunk* hitWallSound;
	Mix_Chunk* hitBarSound;
	Mix_Chunk* hitBlockSound;
	Mix_Chunk* clearSound;
};

