#pragma once
#include <cstdint>

/// <summary>
/// Componentクラス
/// </summary>
class Component
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="owner"> 所有者(Actor*) </param>
	/// <param name="updateOrder"> 更新順序 </param>
	/// updateOrderが小さいコンポーネントほど早く更新される
	Component(class Actor* owner, int updateOrder = 100);
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~Component();

	/// <summary>
	/// コンポーネントをデルタタイムで更新
	/// </summary>
	/// <param name="deltaTime"> デルタタイム </param>
	virtual void Update(float deltaTime);

	/// <summary>
	/// 入力用関数
	/// </summary>
	/// <param name="keyState"> キーボードの状態 </param>
	virtual void ProcessInput(const uint8_t* keyState){}

	/// <summary>
	/// ワールド変換が更新されたら呼ばれる
	/// </summary>
	virtual void OnUpdateWorldTransform() { }

	// getter / setter
	int GetUpdateOrder() const { return mUpdateOrder; }

protected:
	class Actor* mOwner;	// 所有アクター
	int mUpdateOrder;		// コンポーネントの更新順序
};