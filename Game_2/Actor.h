#pragma once
#include <vector>

struct Vector2
{
	float x;
	float y;
};

/// <summary>
/// Actorクラス(基底クラス)
/// </summary>
class Actor
{
public:
	// アクターの状態管理用
	enum State
	{
		EActive,
		EPaused,
		EDead
	};

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="game"> Gameポインタ </param>
	Actor(class Game* game);
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~Actor();

	/// <summary>
	/// 更新(ゲームから呼び出される)...オーバーライド不可
	/// </summary>
	/// <param name="deltaTime"> デルタタイム </param>
	void Update(float deltaTime);
	/// <summary>
	/// アクターが持つ全コンポーネントを更新...オーバーライド不可
	/// </summary>
	/// <param name="deltaTime"> デルタタイム </param>
	void UpdateComponents(float deltaTime);
	/// <summary>
	/// アクター独自の更新処理...オーバーライド可能
	/// </summary>
	/// <param name="deltaTime"> デルタタイム </param>
	virtual void UpdateActor(float deltaTime);

	// getter / setter
	const Vector2& GetPosition() const { return mPosition; }
	void SetPosition(const Vector2& pos) { mPosition = pos; }
	float GetScale() { return mScale; }
	void SetScale(float scale) { mScale = scale; }
	float GetRotation() const { return mRotation; }
	void SetRotation(float rotation) { mRotation = rotation; }

	State GetState() const { return mState; }
	void SetState(State state) { mState = state; }

	class Game* GetGame() { return mGame; }


	/// <summary>
	/// コンポーネントの追加
	/// </summary>
	/// <param name="component"> Componentポインタ </param>
	void AddComponent(class Component* component);
	/// <summary>
	/// コンポーネントの削除
	/// </summary>
	/// <param name="component"> Componentポインタ </param>
	void RemoveComponent(class Component* component);

private:
	// アクターの状態
	State mState;
	
	// 座標変換
	Vector2 mPosition;	// アクターの中心位置
	float mScale;		// アクターのスケール(1.0f = 100%)
	float mRotation;	// 回転の角度(ラジアン)

	// アクターが持つコンポーネント
	std::vector<class Component*> mComponents;
	class Game* mGame;
};