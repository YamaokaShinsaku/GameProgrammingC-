#include "Actor.h"
#include "Game.h"

Actor::Actor(Game* game)
	: mState(EActive)
	, mPosition()
	, mScale(1.0f)
	, mRotation(0.0f)
	, mGame(game)
{
	// �A�N�^�[��ǉ�
	mGame->AddActor(this);
}

Actor::~Actor()
{
	// �A�N�^�[���폜
	mGame->RemoveActor(this);
	// �R���|�[�l���g���폜
	// RemoveActor���Ăяo�����̂ŁA�ʂ̎�ނ̃��[�v���g��
	while (!mComponents.empty())
	{
		delete mComponents.back();
	}
	
}

void Actor::Update(float deltaTime)
{
}

void Actor::UpdateComponents(float deltaTime)
{
}

void Actor::UpdateActor(float deltaTime)
{
}

void Actor::AddComponent(Component* component)
{
}

void Actor::RemoveComponent(Component* component)
{
}
