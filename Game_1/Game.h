#pragma once
#include "SDL/SDL.h"

/// <summary>
/// x,y両方の成分を持つ
/// </summary>
struct Vector2
{
	float x;
	float y;
};

/// <summary>
/// Game クラス
/// </summary>
class Game
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Game();
	/// <summary>
	/// ゲームを初期化する
	/// </summary>
	/// <returns> 初期化に成功したら true/失敗したら false </returns>
	bool Initialize();
	/// <summary>
	/// ゲームオーバーまでゲームループを実行する
	/// </summary>
	void RunLoop();
	/// <summary>
	/// ゲームをシャットダウンする
	/// </summary>
	void Shutdown();

private:
	//ゲームループのためのヘルパー関数群
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();

	// SDLが作るウィンドウ
	SDL_Window* mWindow;
	// 2D描画
	SDL_Renderer* mRenderer;
	// ゲーム開始からの経過時間
	Uint32 mTicksCount;
	// ゲームの続行を指示する
	bool mIsRunning;


	// パドル座標
	Vector2 mPaddlePos;
	// ボール座標
	Vector2 mBallPos;
	// ボールの速度
	Vector2 mBallVel;
	// パドルの移動方向
	int mPaddleDir;
};