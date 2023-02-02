#include "AIComponent.h"
#include "Actor.h"
#include "AIState.h"
#include <SDL/SDL_log.h>

AIComponent::AIComponent(Actor* owner)
	: Component(owner)
	, mCurrentState(nullptr)
{

}

void AIComponent::Update(float deltaTime)
{
	// 現在の状態があれば、Updateを呼び出す
	if (mCurrentState)
	{
		mCurrentState->Update(deltaTime);
	}
}

void AIComponent::ChangeState(const std::string& name)
{
	// まず現在の状態を抜け出せる
	if (mCurrentState)
	{
		mCurrentState->OnExit();
	}

	// 新しい状態を連想配列から探す
	auto iter = mStateMap.find(name);
	if (iter != mStateMap.end())
	{
		mCurrentState = iter->second;
		// 新しい状態に入る
		mCurrentState->OnEnter();
	}
	else
	{
		// ログを表示
		SDL_Log("AIState %s の状態はありません", name.c_str());
		mCurrentState = nullptr;
	}
}

void AIComponent::RegisterState(AIState* state)
{
	mStateMap.emplace(state->GetName(), state);
}
