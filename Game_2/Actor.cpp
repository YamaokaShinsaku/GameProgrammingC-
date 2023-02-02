#include "Actor.h"
#include "Game.h"

Actor::Actor(Game* game)
	: mState(EActive)
	, mPosition()
	, mScale(1.0f)
	, mRotation(0.0f)
	, mGame(game)
{
	// アクターを追加
	mGame->AddActor(this);
}

Actor::~Actor()
{
	// アクターを削除
	mGame->RemoveActor(this);
	// コンポーネントを削除
	// RemoveActorが呼び出されるので、別の種類のループを使う
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
