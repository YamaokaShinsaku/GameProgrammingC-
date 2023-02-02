#include "Game.h"
#include "Actor.h"
#include "SpriteComponent.h"
#include "SDL/SDL_image.h"
#include <algorithm>
#include "Random.h"
#include "AIState.h"
#include "AIComponent.h"
#include "Grid.h"
#include "Enemy.h"

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

	Random::Init();
	LoadData();
	mTicksCount = SDL_GetTicks();

	return true;
}

void Game::Shutdown()
{
	UnloadData();
	IMG_Quit();
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
						  actor);
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
					 actor);
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
	// ソート済みの配列で挿入点を見つける
	// (自分より順序の高い最初の要素の位置)
	int mDrawOrder = sprite->GetDrawOrder();
	auto iter = mSprites.begin();
	for (;
		iter != mSprites.end();
		++iter)
	{
		if (mDrawOrder < (*iter)->GetDrawOrder())
		{
			break;
		}
	}

	// イテレーターの位置の前に要素を挿入する
	mSprites.insert(iter, sprite);
}

void Game::RemoveSprite(SpriteComponent* sprite)
{
	// 順序が崩れるので、スワップはできない
	auto iter = std::find(mSprites.begin(), mSprites.end(), sprite);
	mSprites.erase(iter);
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

Enemy* Game::GetNearestEnemy(const Vector2& pos)
{
	Enemy* best = nullptr;

	if (mEnemies.size() > 0)
	{
		best = mEnemies[0];

		float bestDistSq = (pos - mEnemies[0]->GetPosition()).LengthSq();
		for (size_t i = 1; i < mEnemies.size(); i++)
		{
			float newDistSq = (pos - mEnemies[i]->GetPosition()).LengthSq();
			if (newDistSq < bestDistSq)
			{
				bestDistSq = newDistSq;
				best = mEnemies[i];
			}
		}
	}

	return best;
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
	const Uint8* keyState = SDL_GetKeyboardState(NULL);
	// Escキーが押されていても、ループを終了
	if (keyState[SDL_SCANCODE_ESCAPE])
	{
		mIsRunning = false;
	}

	//--------------------------------
	if (keyState[SDL_SCANCODE_B])
	{
		mGrid->BuildTower();
	}

	int x, y;
	Uint32 buttons = SDL_GetMouseState(&x, &y);
	if (SDL_BUTTON(buttons) & SDL_BUTTON_LEFT)
	{
		mGrid->ProcessClick(x, y);
	}
	//---------------------------------

	// 全アクターのProcessInputを呼び出す
	mUpdatingActors = true;
	for (auto actor : mActors)
	{
		actor->ProcessInput(keyState);
	}
	mUpdatingActors = false;
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
		34,		// R
		139,		// G
		34,		// B
		255		// A(アルファ値)
	);
	// バックバッファを現在の描画色でクリア
	SDL_RenderClear(mRenderer);

	// 全てのSpriteComponentsを描画する
	for (auto sprite : mSprites)
	{
		sprite->Draw(mRenderer);
	}

	// フロントバッファとバックバッファを交換する
	SDL_RenderPresent(mRenderer);
}

void Game::LoadData()
{
	mGrid = new Grid(this);

	//// test
	//Actor* a = new Actor(this);
	//// AIComponentを作成
	//AIComponent* aic = new AIComponent(a);
	//// 状態をAIComponentに登録
	//aic->RegisterState(new AIPatrol(aic));
	//aic->RegisterState(new AIDeath(aic));
	//aic->RegisterState(new AIAttack(aic));
	//// AIComponentをPatrolで初期化
	//aic->ChangeState("Patrol");
}

void Game::UnloadData()
{
	// Delete actors
	// Because ~Actor calls RemoveActor, have to use a different style loop
	while (!mActors.empty())
	{
		delete mActors.back();
	}

	// Destroy textures
	for (auto i : mTextures)
	{
		SDL_DestroyTexture(i.second);
	}
	mTextures.clear();
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
