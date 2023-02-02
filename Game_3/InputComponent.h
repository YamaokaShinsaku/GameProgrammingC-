#pragma once
#include "MoveComponent.h"
#include <cstdint>

/// <summary>
/// キー入力で所有アクターの前進、後退、回転を制御する
/// </summary>
class InputComponent : public MoveComponent
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="owner"> 所有アクター(Actor*) </param>
	InputComponent(class Actor* owner);

	/// <summary>
	/// 入力用関数
	/// </summary>
	/// <param name="keyState"> キーボードの状態 </param>
	void ProcessInput(const uint8_t* keyState)override;

	// getter / setter
	float GetMaxForward() const { return mMaxForwardSpeed; }
	float GetMaxAngular() const { return mMaxAngularSpeed; }
	int GetForwardKey() const { return mForwardKey; }
	int GetBackKey() const { return mBackKey; }
	int GetClockwiseKey() { return mClockwiseKey; }
	int GetCounterClockwiseKey() { return mCounterClockwiseKey; }

	void SetMaxForwardSpeed(float speed) { mMaxForwardSpeed = speed; }
	void SetMaxAngularSpeed(float speed) { mMaxAngularSpeed = speed; }
	void SetForwardKey(int key) { mForwardKey = key; }
	void SetBackKey(int key) { mBackKey = key; }
	void SetClockwiseKey(int key) { mClockwiseKey = key; }
	void SetCounterClockwiseKey(int key) { mCounterClockwiseKey = key; }

private:
	// 前進 / 回転の最大速度
	float mMaxForwardSpeed;
	float mMaxAngularSpeed;

	// 前進 / 後退のためのキー
	int mForwardKey;
	int mBackKey;

	// 回転運動のためのキー
	int mClockwiseKey;
	int mCounterClockwiseKey;
};