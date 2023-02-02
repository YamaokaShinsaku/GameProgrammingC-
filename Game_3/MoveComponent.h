#pragma once
#include "Component.h"

/// <summary>
/// アクターを一定のスピードで前進させる
/// </summary>
class MoveComponent : Component
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="owner"> 所有者(Actor*) </param>
	/// <param name="updateOrder"> 描画順序 </param>
	/// updateOrderの値が小さいほど、先に更新される
	MoveComponent(class Actor* owner, int updateOrder = 10);

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="deltaTime"> デルタタイム </param>
	void Update(float deltaTime)override;

	// getter / setter
	float GetAngularSpeed() const { return mAngularSpeed; }
	float GetForwardSpeed() const { return mForwardSpeed; }
	void SetAngularSpeed(float speed) { mAngularSpeed = speed; }
	void SetForwardSpeed(float speed) { mForwardSpeed = speed; }

private:
	// 回転を制御する（ラジアン / 秒）
	float mAngularSpeed;
	// 前進運動を制御する（単位長 / 秒）
	float mForwardSpeed;
};