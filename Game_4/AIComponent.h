#pragma once
#include "Component.h"
#include <unordered_map>
#include <string>

class AIComponent : public Component
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="owner"> 所有アクター(Actor*) </param>
	AIComponent(class Actor* owner);

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="deltaTime"> デルタタイム </param>
	void Update(float deltaTime)override;
	/// <summary>
	/// ステートマシーンの遷移
	/// </summary>
	/// <param name="name"> ステートの名前 </param>
	void ChangeState(const std::string& name);

	/// <summary>
	/// 新たな状態を連想配列に登録する
	/// </summary>
	/// <param name="state"> 状態 </param>
	void RegisterState(class AIState* state);

private:
	// 状態の名前とAIStateのインスタンスを対応付ける
	std::unordered_map<std::string, class AIState*> mStateMap;
	// 現在の状態
	class AIState* mCurrentState;

};

