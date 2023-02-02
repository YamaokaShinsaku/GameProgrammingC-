#include "Game.h"

const int thickness = 15;	// 長方形描画用
const float paddleH = 100.0f;

Game::Game()
	:mWindow(nullptr)
	,mIsRunning(true)
	,mRenderer(nullptr)
	,mTicksCount(0)
	,mPaddleDir(0)
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

	// 
	mPaddlePos.x = 10.0f;
	mPaddlePos.y = 768.0f / 2.0f;
	mBallPos.x = 1024.0f / 2.0f;
	mBallPos.y = 768.0f / 2.0f;
	mBallVel.x = -200.0f;
	mBallVel.y = 235.0f;

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

	// パドルの移動方向更新 W/Sキー
	mPaddleDir = 0;
	if (state[SDL_SCANCODE_W])
	{
		mPaddleDir -= 1;
	}
	if (state[SDL_SCANCODE_S])
	{
		mPaddleDir += 1;
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

	// パドル座標をデルタタイムに基づいて更新
	if ( mPaddleDir != 0)
	{
		mPaddlePos.y += mPaddleDir * 300.0f * deltaTime;
		// パドルが画面から出ないようにする
		if (mPaddlePos.y < (paddleH / 2.0f + thickness))
		{
			mPaddlePos.y = paddleH / 2.0f + thickness;
		}
		else if (mPaddlePos.y > (768.0f - paddleH / 2.0f - thickness))
		{
			mPaddlePos.y = 768.0f - paddleH / 2.0f - thickness;
		}
	}

	// ボール座標を速度に基づいて更新
	mBallPos.x += mBallVel.x * deltaTime;
	mBallPos.y += mBallVel.y * deltaTime;

	// バウンド
	// ボールがパドルと当たった時
	float diff = mPaddlePos.y - mBallPos.y;
	//
	diff = (diff > 0.0f) ? diff : -diff;
	if (
		// ｙの差が十分に小さく
		diff <= paddleH / 2.0f &&
		// ボールが正しいｘ位置にあり
		mBallPos.x <= 25.0f && mBallPos.x >= 20.0f &&
		// ボールが左向きに動いていれば
		mBallVel.x < 0.0f
		)
	{
		mBallVel.x *= -1.0f;
	}

	// ボールが壁に衝突したら
	// 上部
	if (mBallPos.y <= thickness && mBallVel.y < 0.0f)
	{
		mBallVel.y *= -1;
	}
	// 下部
	else if (mBallPos.y >= 768 - thickness && mBallVel.y > 0.0f)
	{
		mBallVel.y *= -1;
	}
	// 右
	else if (mBallPos.x >= 1024.0f - thickness && mBallVel.x > 0.0f)
	{
		mBallVel.x *= -1;
	}
	
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

	// 長方形の描画
	SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);

	// 壁を描画
	// 上部
	SDL_Rect wall{
		0,			// 左上隅のｘ
		0,			// 左上隅のｙ
		1024,		// 幅
		thickness	// 高さ
	};
	SDL_RenderFillRect(mRenderer, &wall);

	// 下
	wall.y = 768 - thickness;
	SDL_RenderFillRect(mRenderer, &wall);

	// 右
	wall.x = 1024 - thickness;
	wall.y = 0;
	wall.w = thickness;
	wall.h = 1024;
	SDL_RenderFillRect(mRenderer, &wall);


	// パドル描画
	SDL_Rect paddle{
		static_cast<int>(mPaddlePos.x),
		static_cast<int>(mPaddlePos.y - paddleH / 2),
		thickness,
		paddleH
	};
	SDL_RenderFillRect(mRenderer, &paddle);

	// ボール描画
	SDL_Rect ball{
		static_cast<int>(mBallPos.x - thickness / 2),
		static_cast<int>(mBallPos.y - thickness / 2),
		thickness,
		thickness
	};
	SDL_RenderFillRect(mRenderer, &ball);

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
