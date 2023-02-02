#include "AIState.h"
#include "AIComponent.h"
#include <SDL/SDL_log.h>

void AIPatrol::Update(float deltaTime)
{
	// ログを表示
	SDL_Log("Updating %s State", GetName());
	// 自分が死んだかをチェック
	bool dead = true;
	if (dead)
	{
		// 状態の変更をAIコンポーネントに指示する
		mOwner->ChangeState("Death");
	}
}

void AIPatrol::OnEnter()
{
	// ログを表示
	SDL_Log("Entering %s State", GetName());
}

void AIPatrol::OnExit()
{
	// ログを表示
	SDL_Log("Exiting %s State", GetName());
}

void AIDeath::Update(float deltaTime)
{
	// ログを表示
	SDL_Log("Updating %s State", GetName());
}

void AIDeath::OnEnter()
{
	// ログを表示
	SDL_Log("Entering %s State", GetName());
}

void AIDeath::OnExit()
{
	// ログを表示
	SDL_Log("Exiting %s State", GetName());
}

void AIAttack::Update(float deltaTime)
{
	// ログを表示
	SDL_Log("Updating %s State", GetName());
}

void AIAttack::OnEnter()
{
	// ログを表示
	SDL_Log("Entering %s State", GetName());
}

void AIAttack::OnExit()
{
	// ログを表示
	SDL_Log("Exiting %s State", GetName());
}
