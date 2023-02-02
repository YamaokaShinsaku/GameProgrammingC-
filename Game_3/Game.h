#pragma once
#include "SDL/SDL.h"
#include <vector>
#include <string>
#include <unordered_map>

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

	/// <summary>
	/// 対象のアクターを追加する
	/// </summary>
	/// <param name="actor"> Actorポインタ </param>
	/// 受けとったアクターを、mPendingActors か mActorsに追加
	/// どちらに追加するかはmUpdatingActorsに依存する
	void AddActor(class Actor* actor);
	/// <summary>
	/// 対象のアクターを削除する
	/// </summary>
	/// <param name="actor"> Actorポインタ </param>
	/// 対象は、mActors か mPendingActors のどちらかにいる
	void RemoveActor(class Actor* actor);

	/// <summary>
	/// スプライトの追加
	/// </summary>
	/// <param name="sprite"> Spriteポインタ </param>
	void AddSprite(class SpriteComponent* sprite);
	/// <summary>
	/// スプライトの削除
	/// </summary>
	/// <param name="sprite"> Spriteポインタ </param>
	void RemoveSprite(class SpriteComponent* sprite);

	/// <summary>
	/// テクスチャファイルパスを受け取り、対応するポインターを返す
	/// </summary>
	/// <param name="fileName">	ファイル名 </param>
	/// <returns> SDL_Textureのポインタを返す </returns>
	SDL_Texture* GetTexture(const std::string& fileName);


	// -------------------------------------------
	void AddAsteroid(class Asteroid* ast);
	void RemoveAsteroid(class Asteroid* ast);
	std::vector<class Asteroid*>& GetAsteroids() { return mAsteroids; }

private:
	//ゲームループのためのヘルパー関数群
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	void LoadData();
	void UnloadData();

	//Actorへのポインタを格納
	std::vector<class Actor*> mActors;	// ゲーム内のすべてのアクター
	std::vector<class Actor*> mPendingActors;	// 待ち状態のアクター群

	// ロードされたテクスチャのマップ
	std::unordered_map<std::string, SDL_Texture*> mTextures;

	// すべてのSpriteComponentsを描画
	std::vector<class SpriteComponent*> mSprites;

	// SDLが作るウィンドウ
	SDL_Window* mWindow;
	// 2D描画
	SDL_Renderer* mRenderer;
	// ゲーム開始からの経過時間
	Uint32 mTicksCount;
	// ゲームの続行を指示する
	bool mIsRunning;
	// mActorsを更新しているかどうか
	bool mUpdatingActors;

	class Ship* mShip;
	std::vector<class Asteroid*> mAsteroids;

};