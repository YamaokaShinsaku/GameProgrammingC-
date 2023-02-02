#include "AIState.h"
#include "AIComponent.h"
#include <SDL/SDL_log.h>

void AIPatrol::Update(float deltaTime)
{
	// ���O��\��
	SDL_Log("Updating %s State", GetName());
	// ���������񂾂����`�F�b�N
	bool dead = true;
	if (dead)
	{
		// ��Ԃ̕ύX��AI�R���|�[�l���g�Ɏw������
		mOwner->ChangeState("Death");
	}
}

void AIPatrol::OnEnter()
{
	// ���O��\��
	SDL_Log("Entering %s State", GetName());
}

void AIPatrol::OnExit()
{
	// ���O��\��
	SDL_Log("Exiting %s State", GetName());
}

void AIDeath::Update(float deltaTime)
{
	// ���O��\��
	SDL_Log("Updating %s State", GetName());
}

void AIDeath::OnEnter()
{
	// ���O��\��
	SDL_Log("Entering %s State", GetName());
}

void AIDeath::OnExit()
{
	// ���O��\��
	SDL_Log("Exiting %s State", GetName());
}

void AIAttack::Update(float deltaTime)
{
	// ���O��\��
	SDL_Log("Updating %s State", GetName());
}

void AIAttack::OnEnter()
{
	// ���O��\��
	SDL_Log("Entering %s State", GetName());
}

void AIAttack::OnExit()
{
	// ���O��\��
	SDL_Log("Exiting %s State", GetName());
}
