#include "Laser.h"
#include "SpriteComponent.h"
#include "MoveComponent.h"
#include "Game.h"
#include "CircleComponent.h"
#include "Asteroid.h"

Laser::Laser(Game* game)
	: Actor(game)
	, mDeathTimer(1.0f)
{
	SpriteComponent* sc = new SpriteComponent(this);
	sc->SetTexture(game->GetTexture("Assets/Laser.png"));

	MoveComponent* mc = new MoveComponent(this);
	mc->SetForwardSpeed(800.0f);

	mCircle = new CircleComponent(this);
	mCircle->SetRadius(11.0f);
}

void Laser::UpdateActor(float deltaTime)
{
	mDeathTimer -= deltaTime;
	if (mDeathTimer <= 0.0f)
	{
		SetState(EDead);
	}
	else
	{
		// 小惑星と交差するか
		for (auto ast : GetGame()->GetAsteroids())
		{
			if (Intersect(*mCircle, *(ast->GetCircle())))
			{
				// レーザーが小惑星と交差するなら、どちらも消えてなくなる
				SetState(EDead);
				ast->SetState(EDead);

				break;
			}
		}
	}
}
