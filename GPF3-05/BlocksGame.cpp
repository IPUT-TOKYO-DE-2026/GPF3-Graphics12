#include "BlocksGame.h"

#include <string>
#include <format>
#include <random>

// Assets
// https://kenney.nl/assets/rolling-ball-assets
// https://youfulca.com/2022/08/06/rpgmaker_system/
// https://fonts.google.com/noto

BlocksGame::BlocksGame()
	: ball(100, 100, 10, 0, 0)
	, text("NotoSansJP-SemiBold.ttf", 64, true) // ボールとテキストの初期化
	, bar("button_blue_s.bmp")
	, remainingText("NotoSansJP-SemiBold.ttf", 24, true)
	, scoreText("NotoSansJP-SemiBold.ttf", 24, true)
	, pin(700, 350, 20, 0, 0)
{
}

void BlocksGame::initUser()
{
	int blockColorScore[RectRowNum] = { 5, 10, 15, 20 }; // ブロックの行ごとのスコア（黄、緑、オレンジ、赤）
	int startX = width / 2;
	int startY = height - RectHeight / 2 - 10;
	// ボールの初期設定
	ball.setColor(255, 255, 255);
	ball.setCenter(startX, height / 2);
	// バーの初期設定
	bar.setColor(255, 255, 255);
	bar.setCenter(startX, startY);
	barX = startX;
	barY = startY;

	pin.setColor(255, 255, 0);

	objects.push_back(&ball);
	objects.push_back(&bar);
	objects.push_back(&pin);

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> dist(0, 2 * M_PI);
	float speed = 2.0f;
	constexpr int EnemyNum = 20;
	for (int i = 0; i < EnemyNum; i++) {
		float angle = dist(gen);
		float speedX = speed * cos(angle);
		float speedY = speed * sin(angle);
		enemys.emplace_back(rand() % width, rand() % height, 20, speedX, speedY);
		enemys.back().setColor(255, 0, 0);
		//objects.push_back(&enemys.back());
	}


	// ブロックの初期設定
	int centerY = RectRowNum * (RectHeight + RectGap) + RectHeight / 2;
	int idx = 0;

	score = 0; // スコアの初期化
	barRemaining = 3; // 残機数
	blockRemaining = RectNum; // ブロック総数

	text.setColor(150, 200, 100);
	text.setCenter(startX, height / 2);

	text.setText(" ");
	objects.push_back(&text);

	remainingText.setColor(255, 255, 255);
	remainingText.setCenter(50, 25);
	
	remainingText.setText(std::format("残機: {}", barRemaining).c_str());
	objects.push_back(&remainingText);

	scoreText.setColor(255, 255, 255);
	scoreText.setCenter(width - 100, 25);
	scoreText.setText(std::format("スコア: {}", score).c_str());
	objects.push_back(&scoreText);

	// 音声ファイルのロード
	gameOverSound = loadSound("fire-sword.wav");
	failedSound = loadSound("cancel-02.wav");
	hitWallSound = loadSound("cursor-02.wav");
	hitBarSound = loadSound("cursor-01.wav");
	hitBlockSound = loadSound("cancel-01.wav");
	clearSound = loadSound("8bit-ME_Victory01.wav");

	// 入力を検出するキーの登録
	addInputKey(SDLK_w);
	addInputKey(SDLK_a);
	addInputKey(SDLK_s);
	addInputKey(SDLK_d);
}

void BlocksGame::drawUser(unsigned char* buff, int mode, int keyLevel, int keyTrigger)
{
	if (getKeyLevel(SDLK_a)) {
		bar.rotate(rot += -0.01f); // バーを左に回転
	}
	if (getKeyLevel(SDLK_d)) {
		bar.rotate(rot += 0.01f); // バーを右に回転
	}

	if (getKeyLevel(SDLK_w)) {
		barY -= barSpeedX; // バーを上に動かす
		bar.setCenter(barX, barY);
	}
	if (getKeyLevel(SDLK_s)) {
		barY += barSpeedX; // バーを下に動かす
		bar.setCenter(barX, barY);
	}

	if (bar.isHit(ball)) { // バーとボールの当たり判定
		ball.speedY *= -1; // 当たったらY方向の速度を反転
		playSound(hitBarSound);
	}

	if (ball.isHit(pin)) {
		ball.speedY *= -1; // 当たったらY方向の速度を反転
		pin.isActive = false; // ピンを非表示にする
		playSound(hitBarSound);
	}

	for (Circle& enemy : enemys) {
		if (enemy.centerY - enemy.radius < 0) { // ボールが上端に到達
			enemy.speedY *= -1; // Y方向の速度を反転
		}
		if (enemy.centerY + enemy.radius >= height) { // ボールが下端に到達
			enemy.speedY *= -1; // Y方向の速度を反転
		}

		if (enemy.centerX + enemy.radius >= width) { // ボールが右端に到達
			enemy.speedX *= -1; // X方向の速度を反転
		}

		if (enemy.centerX - enemy.radius < 0) { // ボールが左端に到達
			enemy.speedX *= -1; // X方向の速度を反転
		}
		enemy.move();
		enemy.draw(buff, width, height);
	}

	for (GameObject* obj : objects) { // 画面内のすべてのオブジェクトを描画する
		obj->draw(buff, width, height);
	}
}