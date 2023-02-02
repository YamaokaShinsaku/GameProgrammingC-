#include "Game.h"


Game::Game()
	:mWindow(nullptr)
	, mIsRunning(true)
	, mRenderer(nullptr)
	, mTicksCount(0)
{
}

bool Game::Initialize()
{
	// SDLライブラリを初期化する
	// SDL_Init の返り値が０でなければ初期化に失敗
	int sdlResult = SDL_Init(SDL_INIT_VIDEO);
	// 初期化に失敗したとき
	if (sdlResult != 0)
	{
		// ログを表示
		SDL_Log("SDLを初期化できません : %s", SDL_GetError());

		return false;
	}

	// ウィンドウ作成
	mWindow = SDL_CreateWindow(
		"OpenGL_Game",	// ウィンドウのタイトル
		100,			// ウィンドウ左上隅のx座標
		100,			// ウィンドウ左上隅のy座標
		1024,			// ウィンドウの幅(width)
		768,			// ウィンドウの高さ(height)
		0				// フラグ(設定しないときは０)
	);
	// ウィンドウの作成に失敗したとき
	if (!mWindow)
	{
		// ログを表示
		SDL_Log("ウィンドウの作成に失敗しました : %s", SDL_GetError());
		return false;
	}

	// SDLレンダラー作成
	mRenderer = SDL_CreateRenderer(
		mWindow,	// 作成するレンダラーの描画対象となるウィンドウ
		-1,			// 通常は -1
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	);
	// SDLレンダラーの作成に失敗したとき
	if (!mRenderer)
	{
		// ログを表示
		SDL_Log("レンダラーの初期化に失敗しました : %s", SDL_GetError());
		return false;
	}

	return true;
}

void Game::Shutdown()
{
	// SDL_Windowを破棄
	SDL_DestroyWindow(mWindow);
	// SDL_Rendererを破棄
	SDL_DestroyRenderer(mRenderer);
	// SDL終了
	SDL_Quit();
}

void Game::ProcessInput()
{
	SDL_Event event;
	// キューにイベントがあれば繰り返す
	while (SDL_PollEvent(&event))
	{
		// イベント処理
		switch (event.type)
		{
			// SDL_QUITイベントなら、ループ終了
		case SDL_QUIT:
			mIsRunning = false;
			break;
		}
	}

	// キーボードの状態を取得する
	const Uint8* state = SDL_GetKeyboardState(NULL);
	// Escキーが押されていても、ループを終了
	if (state[SDL_SCANCODE_ESCAPE])
	{
		mIsRunning = false;
	}

}

void Game::UpdateGame()
{
	// 次のフレームまで16ms経過するまで待つ
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
		;

	// deltaTime...前フレームとの時間の差を秒に変換した値
	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
	// デルタタイムを最大値で制限
	if (deltaTime > 0.05f)
	{
		deltaTime = 0.05f;
	}

	// 時間を更新(次のフレームのため)
	mTicksCount = SDL_GetTicks();

}

void Game::GenerateOutput()
{
	// 描画色を指定
	SDL_SetRenderDrawColor(
		mRenderer,
		0,		// R
		0,		// G
		255,	// B
		255		// A(アルファ値)
	);
	// バックバッファを現在の描画色でクリア
	SDL_RenderClear(mRenderer);


	// フロントバッファとバックバッファを交換する
	SDL_RenderPresent(mRenderer);
}

void Game::RunLoop()
{
	while (mIsRunning)
	{
		// ヘルパー関数を呼び出す
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}
