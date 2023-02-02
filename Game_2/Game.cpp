#include "Game.h"
#include "Actor.h"
#include "SDL/SDL_image.h"

Game::Game()
	: mWindow(nullptr)
	, mIsRunning(true)
	, mRenderer(nullptr)
	, mUpdatingActors(false)
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

	// 画像(png)の生成に失敗したとき
	if (IMG_Init(IMG_INIT_PNG) == 0)
	{
		// ログを表示
		SDL_Log("画像の初期化に失敗しました : %s", SDL_GetError());
		return false;
	}


	mTicksCount = SDL_GetTicks();

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

void Game::AddActor(Actor* actor)
{
	// アクターの更新中なら mPendingActors に追加
	if (mUpdatingActors)
	{
		mPendingActors.emplace_back(actor);
	}
	else
	{
		mActors.emplace_back(actor);
	}
}

void Game::RemoveActor(Actor* actor)
{
	// mPendingActors かどうか
	auto iter = std::find(mPendingActors.begin(), 
						  mPendingActors.end(),
						  mPendingActors.size());
	if (iter != mPendingActors.end())
	{
		// vectorの最後にスワップして削除する
		// ※コピーを削除しないように注意
		std::iter_swap(iter, mPendingActors.end() - 1);
		mPendingActors.pop_back();
	}

	// mActors かどうか
	iter = std::find(mActors.begin(),
					 mActors.end(),
					 mActors.size());
	if (iter != mActors.end())
	{
		// vectorの最後にスワップして削除する
		// ※コピーを削除しないように注意
		std::iter_swap(iter, mActors.end() - 1);
		mActors.pop_back();
	}

}

void Game::AddSprite(SpriteComponent* sprite)
{
}

void Game::RemoveSprite(SpriteComponent* sprite)
{
}

SDL_Texture* Game::GetTexture(const std::string& fileName)
{
	SDL_Texture* tex = nullptr;
	// テクスチャがマップに準備されているかどうか
	auto iter = mTextures.find(fileName);
	if (iter != mTextures.end())
	{
		tex = iter->second;
	}
	else
	{
		// ファイルからロード
		SDL_Surface* surf = IMG_Load(fileName.c_str());
		// テクスチャファイルのロードに失敗したとき
		if (!surf)
		{
			// ログ表示
			SDL_Log("テクスチャのロードに失敗しました : %s", fileName.c_str());
			return nullptr;
		}

		// サーフェスからテクスチャを作成
		tex = SDL_CreateTextureFromSurface(mRenderer, surf);
		SDL_FreeSurface(surf);
		// テクスチャへの変換に失敗
		if (!tex)
		{
			// ログを表示
			SDL_Log("テクスチャへの変換に失敗しました : %s", fileName.c_str());
			return nullptr;
		}

		mTextures.emplace(fileName.c_str(), tex);
	}

	return tex;
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

	// 全てのアクターを更新
	mUpdatingActors = true;
	for (auto actor : mActors)
	{
		actor->Update(deltaTime);
	}
	mUpdatingActors = false;

	// 待ちになっていたアクターを mActors に移動
	for (auto pending : mPendingActors)
	{
		mActors.emplace_back(pending);
	}
	mPendingActors.clear();

	// 死んだアクターを一時配列に追加
	std::vector<Actor*> deadActors;
	for (auto actor : mActors)
	{
		if (actor->GetState() == Actor::EDead)
		{
			deadActors.emplace_back(actor);
		}
	}

	// 死んだアクターを削除する(mActorsから削除される)
	for (auto actor : deadActors)
	{
		delete actor;
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
