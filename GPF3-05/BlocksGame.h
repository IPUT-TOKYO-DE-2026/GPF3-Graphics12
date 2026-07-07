#pragma once

#include <vector>
#include <memory>
#include <windows.h>

#include "FrameBufferEmulator.h"
#include "Circle.h"
#include "FilledRectangle.h"
#include "Sprite.h"
#include "Text.h"

class BlocksGame : public FrameBufferEmulator
{
public:
	enum GameState {
		TitleScreen,
		Playing,
		GameOver,
		Cleared
	};

	BlocksGame();
	~BlocksGame() {}

	void initUser();
	void drawUser(unsigned char* buff, int mode, int keyLevel, int keyTrigger);

private:
	GameState gameState = TitleScreen; // ゲームの状態を保持する変数

	int playerX; // バーのX座標
	int playerY; // バーのY座標
	int playerSpeedX = 3; // バーの横方向の移動スピード

	int playerRemaining;   // プレイヤーの残機
	int enemyRemaining; // 残りの敵の数
	int score;          // スコア

	float rot = 0.0f;

	std::vector<Sprite> enemys;
	std::vector<std::shared_ptr<Sprite>> lightnings;

	Sprite player; // プレイヤー


	std::vector<GameObject*> objects; // すべてのオブジェクトを保持する可変長配列
	Text text;
	Text remainingText;
	Text scoreText;

	DWORD startTime;

	bool isPlaying = false;
	Mix_Chunk* failedSound;
	Mix_Chunk* gameOverSound;
	Mix_Chunk* hitWallSound;
	Mix_Chunk* hitPlayerSound;
	Mix_Chunk* hitBlockSound;
	Mix_Chunk* clearSound;

	void initGame();
};

