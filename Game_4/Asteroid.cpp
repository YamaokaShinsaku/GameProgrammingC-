#include "Asteroid.h"
#include "Game.h"
#include "Random.h"
#include "SpriteComponent.h"
#include "MoveComponent.h"
#include "CircleComponent.h"

Asteroid::Asteroid(Game* game)
	: Actor(game)
	, mCircle(nullptr)
{
	// ランダムな位置 / 向きで初期化する
	Vector2 randPos = Random::GetVector(Vector2::Zero, Vector2(1024.0f, 768.0f));
	SetPosition(randPos);
	SetRotation(Random::GetFloatRange(0.0f, Math::TwoPi));

	// スプライトコンポーネントを作成し、テクスチャを設定する
	SpriteComponent* sc = new SpriteComponent(this);
	sc->SetTexture(game->GetTexture("Assets/Asteroid.png"));

	// 運動コンポーネントを作成し、前進速度を設定する
	MoveComponent* mc = new MoveComponent(this);
	mc->SetForwardSpeed(150.0f);

	// CircleComponentを作成、当たり判定を設定する
	mCircle = new CircleComponent(this);
	mCircle->SetRadius(40.0f);

	// mAsteroidsをgameに追加する
	game->AddAsteroid(this);
}

Asteroid::~Asteroid()
{
	GetGame()->RemoveAsteroid(this);
}
