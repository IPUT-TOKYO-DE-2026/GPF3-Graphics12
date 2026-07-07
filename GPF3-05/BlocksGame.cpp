#include "BlocksGame.h"

#include <string>
#include <format>
#include <random>

#pragma comment(lib, "winmm.lib")

// Assets
// https://kenney.nl/assets/jumper-pack
// https://youfulca.com/2022/08/06/rpgmaker_system/
// https://fonts.google.com/noto

BlocksGame::BlocksGame()
	: text("NotoSansJP-SemiBold.ttf", 64, true) // ボールとテキストの初期化
	, player("spikeMan_jump.bmp")
	, remainingText("NotoSansJP-SemiBold.ttf", 24, true)
	, scoreText("NotoSansJP-SemiBold.ttf", 24, true)
{
}

void BlocksGame::initUser()
{
	int blockColorScore[RectRowNum] = { 5, 10, 15, 20 }; // ブロックの行ごとのスコア（黄、緑、オレンジ、赤）
	int startX = width / 2;
	int startY = height - RectHeight / 2 - 10;

	// バーの初期設定
	player.setColor(255, 255, 255);
	player.setCenter(startX, startY);
	playerX = startX;
	playerY = startY;

	objects.push_back(&player);

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> dist(0, 2 * M_PI); // 0から2πまでの範囲でランダムな角度を生成
	float speed = 1.0f;
	constexpr int EnemyNum = 3;
	for (int i = 0; i < EnemyNum; i++) {
		float angle = dist(gen);
		float speedX = speed * cos(angle);
		float speedY = speed * sin(angle);
		enemys.emplace_back(Sprite());
		enemys.back().setCenter(rand() % width, rand() % height);
		enemys.back().setSpeed(speedX, speedY);
		enemys.back().setColor(255, 0, 0);
		enemys.back().addImageFrame("WingMan/wingMan2.bmp");
		enemys.back().addImageFrame("WingMan/wingMan3.bmp");
		enemys.back().addImageFrame("WingMan/wingMan4.bmp");
		enemys.back().setAnimDuration(0.5f); // アニメーションの総時間を設定
	}

	score = 0; // スコアの初期化
	playerRemaining = 3; // 残機数

	text.setColor(150, 200, 100);
	text.setCenter(startX, height / 2);

	text.setText(" ");
	objects.push_back(&text);

	remainingText.setColor(255, 255, 255);
	remainingText.setCenter(50, 25);
	
	remainingText.setText(std::format("残機: {}", playerRemaining).c_str());
	objects.push_back(&remainingText);

	scoreText.setColor(255, 255, 255);
	scoreText.setCenter(width - 100, 25);
	scoreText.setText(std::format("スコア: {}", score).c_str());
	objects.push_back(&scoreText);

	// 音声ファイルのロード
	gameOverSound = loadSound("fire-sword.wav");
	failedSound = loadSound("cancel-02.wav");
	hitWallSound = loadSound("cursor-02.wav");
	hitPlayerSound = loadSound("cursor-01.wav");
	hitBlockSound = loadSound("cancel-01.wav");
	clearSound = loadSound("8bit-ME_Victory01.wav");

	// 入力を検出するキーの登録
	addInputKey(SDLK_w);
	addInputKey(SDLK_a);
	addInputKey(SDLK_s);
	addInputKey(SDLK_d);
	addInputKey(SDLK_SPACE);
	addInputKey(SDLK_RETURN);

	enemyRemaining = enemys.size(); // 敵の残り数を初期化


	startTime = timeGetTime(); // ゲーム開始時刻を取得(ms)
}

void BlocksGame::initGame()
{
	// ゲームの初期化処理をここに記述
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> dist(0, 2 * M_PI); // 0から2πまでの範囲でランダムな角度を生成
	float speed = 1.0f;

	for (auto& enemy : enemys) {
		float angle = dist(gen);
		float speedX = speed * cos(angle);
		float speedY = speed * sin(angle);
		enemy.isActive = true;
		enemy.setCenter(rand() % width, rand() % height);
		enemy.setSpeed(speedX, speedY);
		enemy.setAnimDuration(0.5f); // アニメーションの総時間を設定
	}

	enemyRemaining = enemys.size(); // 敵の残り数を初期化
}

void BlocksGame::drawUser(unsigned char* buff, int mode, int keyLevel, int keyTrigger)
{
	if (gameState == TitleScreen) {
		text.setText("Enterでスタート");
		if (getKeyTrigger(SDLK_RETURN)) {
			gameState = Playing;
			text.setText(" ");
			startTime = timeGetTime(); // ゲーム開始時刻を取得(ms)
		}
	}
	else if (gameState == GameOver) {
		text.setText("GAME OVER");
		if (getKeyTrigger(SDLK_RETURN)) {
			gameState = Playing;
			initGame();
		}
	}
	else if (gameState == Cleared) {
		text.setText("CLEAR!");
		if (getKeyTrigger(SDLK_RETURN)) {
			gameState = Playing;
			initGame();
		}
	}

	if (GameState::Playing != gameState) {
		text.draw(buff, width, height); // ゲーム中でない場合はテキストを描画
		return; // ゲーム中でない場合は他の処理を行わない
	}

	if (getKeyLevel(SDLK_a)) {
		player.rotate(rot += -0.01f); // バーを左に回転
	}
	if (getKeyLevel(SDLK_d)) {
		player.rotate(rot += 0.01f); // バーを右に回転
	}

	if (getKeyLevel(SDLK_w)) {
		playerY -= playerSpeedX; // バーを上に動かす
		player.setCenter(playerX, playerY);
	}
	if (getKeyLevel(SDLK_s)) {
		playerY += playerSpeedX; // バーを下に動かす
		player.setCenter(playerX, playerY);
	}

	if (getKeyTrigger(SDLK_SPACE)) {
		std::shared_ptr<Sprite> lightning = std::make_shared<Sprite>("lighting_yellow_s.bmp");
		float vecX = cos(rot - M_PI / 2);
		float vecY = sin(rot - M_PI / 2);
		lightning->setCenter(player.centerX + vecX * player.sizeW * 0.6f, player.centerY + vecY * player.sizeH * 0.6f);
		float speed = 1.0f; // ライトニングの速度
		float speedX = speed * vecX;
		float speedY = speed * vecY;
		lightning->rotate(rot); // バーの回転角度に合わせてライトニングを回転
		lightning->setSpeed(speedX, speedY); // バーの回転角度に合わせて速度を設定
		lightnings.push_back(lightning); // オブジェクトリストに追加
	}

	DWORD currentTime = timeGetTime(); // 現在の時間を取得(ms)
	float elapsedTime = (currentTime - startTime) / 1000.0f; // 経過時間を秒単位に変換

	for (std::shared_ptr<Sprite>& lightning : lightnings) {
		lightning->move();
		lightning->draw(buff, width, height);
	}
	for (Sprite& enemy : enemys) {
		if (enemy.centerY - enemy.sizeH / 2 < 0) { // ボールが上端に到達
			enemy.speedY *= -1; // Y方向の速度を反転
		}
		if (enemy.centerY + enemy.sizeH / 2 >= height) { // ボールが下端に到達
			enemy.speedY *= -1; // Y方向の速度を反転
		}

		if (enemy.centerX + enemy.sizeW / 2 >= width) { // ボールが右端に到達
			enemy.speedX *= -1; // X方向の速度を反転
		}

		if (enemy.centerX - enemy.sizeW / 2 < 0) { // ボールが左端に到達
			enemy.speedX *= -1; // X方向の速度を反転
		}
		enemy.setTime(elapsedTime);
		enemy.move();

		for (std::shared_ptr<Sprite>& lightning : lightnings) {
			if (lightning->isHit(enemy)) {
				enemy.isActive = false; // 敵を非表示にする
				lightning->isActive = false; // ライトニングを非表示にする
				playSound(hitBlockSound);
				enemyRemaining--; // 敵の残り数を減らす
				break;

				//if (enemyRemaining <= 0) {
				//	gameState = Cleared; // すべての敵を倒したらゲームクリア
				//	playSound(clearSound);
				//}
			}
		}
		enemy.draw(buff, width, height);
	}

	for (auto it = lightnings.begin(); it != lightnings.end();) {
		if (!(*it)->isActive) {
			it = lightnings.erase(it); // 非アクティブなライトニングを削除
			                           // 削除された要素の次の要素を指すイテレータを返す
		}
		else if ((*it)->centerX < 0 || (*it)->centerX >= width || (*it)->centerY < 0 || (*it)->centerY >= height) {
			it = lightnings.erase(it); // 画面外のライトニングを削除
		} else {
			++it;
		}
	}

	for (GameObject* obj : objects) { // 画面内のすべてのオブジェクトを描画する
		obj->move(); // オブジェクトを移動させる
		obj->draw(buff, width, height);
	}
}