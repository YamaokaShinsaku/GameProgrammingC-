#include "Game.h"
#include <GL/glew.h>
#include <algorithm>
#include "Actor.h"
#include "SpriteComponent.h"
#include "Shader.h"
#include "Texture.h"

//--------------------------
#include "Asteroid.h"
#include "Ship.h"
#include "Random.h"
#include "VertexArray.h"

Game::Game()
	: mWindow(nullptr)
	, mIsRunning(true)
	, mSpriteShader(nullptr)
	, mUpdatingActors(false)
	, mTicksCount(0)
{
}

bool Game::Initialize()
{
	// SDLを初期化する
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
	{
		// ログ表示
		SDL_Log("Unable to Initialize SDL : %s", SDL_GetError());

		return false;
	}

	// OpenGLウィンドウの属性を指定　成功　０/ 失敗　-の値
	// コア OpenGL プロファイルを使う
	SDL_GL_SetAttribute(
		SDL_GL_CONTEXT_PROFILE_MASK,	// 設定する属性
		SDL_GL_CONTEXT_PROFILE_CORE		// この属性の値
	);

	// バージョン3.3を指定
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

	// RGBA 各チャンネル8ビットのカラーバッファを使う
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

	// ダブルバッファを有効にする
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	// ハードウェアアクセラレーションを使う
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

	// ウィンドウ作成
	mWindow = SDL_CreateWindow(
		"OpenGL_Game",		// ウィンドウのタイトル
		100,				// ウィンドウ左上隅のx座標
		100,				// ウィンドウ左上隅のy座標
		1024,				// ウィンドウの幅(width)
		768,				// ウィンドウの高さ(height)
		SDL_WINDOW_OPENGL	// フラグ(設定しないときは０)
	);
	// ウィンドウの作成に失敗したとき
	if (!mWindow)
	{
		// ログを表示
		SDL_Log("Failed to create window : %s", SDL_GetError());
		return false;
	}

	// OpenGL コンテクストを作成
	mContext = SDL_GL_CreateContext(mWindow);

	// GLEW を初期化
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		// ログ表示
		SDL_Log("Failed to Initialize GLEW ");
		return false;
	}

	// 一部のプラットフォームでは GLEW が無害なエラーコードを出ので
	// それをクリアする
	glGetError();

	// シェーダーを呼び出す
	if (!LoadShader())
	{
		// ログ表示
		SDL_Log("Failed to load shaders");
		return false;
	}

	Random::Init();
	CreateSpriteVerts();
	LoadData();
	mTicksCount = SDL_GetTicks();

	return true;
}

void Game::Shutdown()
{
	UnloadData();
	delete mSpriteVerts;
	mSpriteShader->Unload();
	delete mSpriteShader;
	// SDL_GLContext を破棄
	SDL_GL_DeleteContext(mContext);
	// SDL_Windowを破棄
	SDL_DestroyWindow(mWindow);
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

Texture* Game::GetTexture(const std::string& fileName)
{
	Texture* tex = nullptr;
	// テクスチャがマップに準備されているかどうか
	auto iter = mTextures.find(fileName);
	if (iter != mTextures.end())
	{
		tex = iter->second;
	}
	else
	{
		// ファイルからロード
		tex = new Texture();
		if (tex->Load(fileName))
		{
			mTextures.emplace(fileName.c_str(), tex);
		}
		else
		{
			delete tex;
			tex = nullptr;
		}

	}

	return tex;
}

//----------------------------------------------
void Game::AddAsteroid(Asteroid* ast)
{
	mAsteroids.emplace_back(ast);
}

void Game::RemoveAsteroid(Asteroid* ast)
{
	auto iter = std::find(mAsteroids.begin(),
		mAsteroids.end(), ast);
	if (iter != mAsteroids.end())
	{
		mAsteroids.erase(iter);
	}
}
//----------------------------------------------

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
		pending->ComputeWorldTransform();
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
	// クリアカラーを設定
	glClearColor(0.86f, 0.86f, 0.86f, 1.0f);
	// カラーバッファをクリア
	glClear(GL_COLOR_BUFFER_BIT);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// スプライトのシェーダーと頂点配列のオブジェクトをアクティブ化
	mSpriteShader->SetActive();
	mSpriteVerts->SetActive();

	// 全てのスプライトを描画
	for (auto sprite : mSprites)
	{
		sprite->Draw(mSpriteShader);
	}

	// バッファを交換(シーンを表示)
	SDL_GL_SwapWindow(mWindow);
}

bool Game::LoadShader()
{
	mSpriteShader = new Shader();
	if (!mSpriteShader->Load("Shaders/Sprite.vert", "Shaders/Sprite.frag"))
	{
		return false;
	}

	mSpriteShader->SetActive();

	// ビュー射影行列を作成し、設定する
	Matrix4 viewProj = Matrix4::CreateSimpleViewProj(1024.f, 768.f);
	mSpriteShader->SetMatrixUniform("uViewProj", viewProj);

	return true;
}

void Game::CreateSpriteVerts()
{
	float vertices[] = {
		-0.5f,  0.5f, 0.f, 0.f, 0.f, // 頂点0  左上
		 0.5f,  0.5f, 0.f, 1.f, 0.f, // 頂点1  右上
		 0.5f, -0.5f, 0.f, 1.f, 1.f, // 頂点2  右下
		-0.5f, -0.5f, 0.f, 0.f, 1.f  // 頂点3  左下
	};

	unsigned int indices[] = {
		0,1,2, // 一つ目の三角形 (頂点1,2,3)
		2,3,0  // 二つ目の三角形 (頂点2,3,0)
	};

	mSpriteVerts = new VertexArray(vertices, 4, indices, 6);
}

void Game::LoadData()
{
	mShip = new Ship(this);
	//mShip->SetPosition(Vector2(512.0f, 384.0f));
	mShip->SetRotation(Math::PiOver2);

	const int numAsteroids = 20;
	for (int i = 0; i < numAsteroids; i++)
	{
		new Asteroid(this);
	}
}

void Game::UnloadData()
{
	// アクターを削除
	while (!mActors.empty())
	{
		delete mActors.back();
	}

	// テクスチャ―を削除
	for (auto i : mTextures)
	{
		i.second->Unload();
		delete i.second;
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
